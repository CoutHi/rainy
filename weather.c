#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Function prototype for a function to check if malloc was succesfull
int checkmalloc(char* x);

int main()
{
  char *api_key; 
  api_key = malloc(sizeof(char)*512);

  // An api key from pirateweather is needed to use the program
  printf("Please input api key: ");
  scanf("%s", api_key);
  printf("\n");
  clearBuffer();
  
  char* latitude;
  latitude = malloc(sizeof(char)*256);
  char* longitude; 
  longitude = malloc(sizeof(char)*256);
  
  checkmalloc(latitude);
  checkmalloc(longitude);

  char url_pirate_template[] = "https://api.pirateweather.net"; // pirateweather query url template
  char* url_pirate; 
  url_pirate = malloc(sizeof(char)*1024);
  
  checkmalloc(url_pirate);

  char url_nominatim_template[] = "https://nominatim.openstreetmap.org/search?q="; // url to get latitude etc.
  char url_nominatim[512]; 

  char* city;
  city = malloc(sizeof(char)*256);
  checkmalloc(city);

  // Asking the user for city-country information
  printf("Which city would you like to look up? (format= city/country(or)/state) "); 
  scanf("%s", city);
  clearBuffer();
  
  // String to save commands into, which we later execute from
  char command[1024]; 

  // Constucting the url which will be used to make the API call
  snprintf(url_nominatim, sizeof(url_nominatim), "%s%s&format=json&addressdetails=1", url_nominatim_template, city); 
  // Constructing the command which we'll use to download the json file containing the coordinates from the API
  snprintf(command, sizeof(command), "curl '%s' --output latlong.json", url_nominatim); 
  printf("\nDownloading json file containing information about %s\n\n", city);
  // Printing the command used, useful for debuginng
  printf("%s\n", command);
  // Execute the command
  system(command); 

  FILE* fileptr;
  
  // Opening the file containing coordinates
  fileptr = fopen("latlong.json", "r");
  
  // Checking if the file was opened properly
  if(fileptr == NULL)
  {
    printf("File could not be opened! The program will now exit.");
    return 1;
  }

  // Array which will containt the contents of the json file
  char* fileLatLong;
  fileLatLong = malloc(sizeof(char)*5128);
  

  // Copy the contents of the API file into a string
  while(fgets(fileLatLong, 5128, fileptr) != NULL)
  {
    fgets(fileLatLong, 5128, fileptr);
//    printf("%s", fileLatLong);
  }

  // Close the json file after use
  fclose(fileptr);

  int i = 0;
  int j = 0;

  // Loops to get the coordinates from the json file
  for(i = 0; fileLatLong[i] != '\0'; i++)
  {
    if(fileLatLong[i] == '"' && strncmp(&fileLatLong[i+1], "lon", 3) == 0) 
    {
      i += 7;

      for(j = 0; fileLatLong[i] != '"'; j++, i++)
      {
        longitude[j] = fileLatLong[i];
      }
    }
  }

  for(i = 0; fileLatLong[i] != '\0'; i++)
  {
    if(fileLatLong[i] == '"' && strncmp(&fileLatLong[i+1], "lat", 3) == 0) 
    {
      i += 7;

      for(j = 0; fileLatLong[i] != '"'; j++, i++)
      {
        latitude[j] = fileLatLong[i];
      }
    }
  }
  // Free the array containing the json contents for the coordinates
  free(fileLatLong);
  
  // Terminate the strings
  longitude[i] = '\0';
  latitude[i] = '\0';

  printf("\n\nlongitude = %s", longitude);
  printf("\nlatitude = %s\n\n", latitude);

  // Construct the url, which will be used to get weather information using the acquired coordinates
  snprintf(url_pirate, 1024 ,"%s/forecast/%s/%s,%s?&units=si" ,url_pirate_template, api_key, latitude, longitude);
//  printf("\nURL for API call: %s\n", url_pirate);
  // Free the strings which will not be used anymore
  free(api_key);
  free(latitude);
  free(longitude);

  // Empty the string used to input commands into the terminal
  strcpy(command, "");
  printf("Downloading the file containing weather information\n");
  // Construct the new command to get the json information from the weather api
  snprintf(command, 1024, "curl '%s' --output forecast.json", url_pirate);
  // Print the command which will be used, useful for debugging
//  printf("Command: %s\n", command);
  // Execute the command
  system(command);
  // Free the url string after use
  free(url_pirate);

  // Reset the variables used for indexing
  i = 0;
  j = 0;


  // Open the json file
  fileptr = fopen("forecast.json", "r");

  // Check if the file was opened properly
  if(fileptr == NULL)
  {
    printf("File could not be opened, the program will now exit.");
    return 1;
  }

  // String arrays to display weather information
  char visibility[32];
  char summary[32];
  char humidity[32];
  char precipProbability[32];
  char windspeed[32];
  char temperature[32]; 

  // String to store part of the json gathered from the api
  char* tempfile;
  tempfile = malloc(sizeof(char)* 1048);
  checkmalloc(tempfile);

  i = 0;
  int c = 0;

  // Copying the first 1048 characters of the json file into a string
  while((c = fgetc(fileptr))!= EOF && i < 1048)
  {
    tempfile[i] = c;
    i++;
  }
  // Close the json file
  fclose(fileptr);

  i = 0;
  j = 0;

  // Loop to find the desired value from the string acquired from the json file and extract it's value
  for(i = 0; tempfile[i] != '\0'; i++)
  {
    if(tempfile[i] == '"' && strncmp(&tempfile[i+1], "temperature", 7) == 0)
    {
      
      i+=14;  

      for(j = 0; tempfile[i] != ',';j++ ,i++)
      {
        temperature[j] = tempfile[i]; 
        c = j;
      }
    }
  }
  temperature[c+1] = '\0';


  for(i = 0; tempfile[i] != '\0'; i++)
  {
    if(tempfile[i] == '"' && strncmp(&tempfile[i+1], "summary", 7) == 0)
    {
      
      i+=11;  

      for(j = 0; tempfile[i] != '"';j++ ,i++)
      {
        summary[j] = tempfile[i]; 
        c = j;
      }
    }
  }
  summary[c+1] = '\0';


  for(i = 0; tempfile[i] != '\0'; i++)
  {
    if(tempfile[i] == '"' && strncmp(&tempfile[i+1], "precipProbability", 17) == 0)
    {
      i+=20;  

      for(j = 0; tempfile[i] != ','; j++ ,i++)
      {
        precipProbability[j] = tempfile[i]; 
        c = j;
      }
    }
  }
  precipProbability[c+1] = '\0';


  // Printing out the string containing the json information, useful for debuginng
//  printf("\nTHE WHOLE FILE: %s",tempfile);
  free(tempfile);
  tempfile = NULL;

  // Printing out weather information
  printf("\n\nThe Weather in %s: ", city);
  printf("\n\nTemperature: %s\nPrecipication: %s\nSummary: %s\n",temperature, precipProbability,summary);

  free(city);

  return 0;
}

// Function to check if memory for a dynamic array was properly initialized
int checkmalloc(char* x)
{
  if (x == NULL)
  {
    printf("Memory allocation unsuccessfull!");
    free(x);
    return 1;
  }
  else 
  {
    return 0;
  }
}
