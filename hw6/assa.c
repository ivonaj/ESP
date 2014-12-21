//-----------------------------------------------------------------------------
// assa.c
//
// A program to calculate the flight path of a bullet shot with a specific
// speed and angle. 
// The calculated flightpath will be printed to a bitmap.
// The program is called using the parameters:
// ./assa [float:angle] [float:speed] [output_filename] {config_filename}
// The square bracket are mandatory while the curly brackets are optional.
// 
// The config file in plain text can add further options to the calculation
// such as the size of the bitmap, the calculated points per second, gravity or
// if and how strong wind should blow.
//
// Group: Group ID: 10619, study assistant Daniel Ellmayr
//
// Authors: Peter Lorenz 1114658
//          Benjamin Franz Herber Rauch 1431077
//          Jan Schlacher 1430256
//-----------------------------------------------------------------------------
//

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>


//MACROS
#define CHECK_ERROR(error, points, pixel) {\
  if(error)\
  {\
    free(points);\
    free(pixel);\
    return errorHandling(error);\
  }}

#define ARRAY_ACCESS(x, y, width) (x + y * width)

//returns
#define SUCCESS             0
#define ERROR_ARGUMENTS     1
#define ERROR_OUT_OF_MEMORY 2
#define ERROR_OPEN_FILE     3
#define ERROR_SPEED         4

//error messages
#define ERROR_MSG_ARG         "usage: ./assa [float:angle] [float:speed] \
[output_filename] {optional:config_filename}\n"
#define ERROR_MSG_OUT_OF_MEM  "out of memory\n"
#define ERROR_MSG_OPEN_FILE   "error: couldn't open file\n"
#define ERROR_MSG_SPEED       "error: speed must be > 0\n"

#define ERROR_MSG_CONFIG_FILE "missing or incorrect entrie(s) - \
using default values\n"
#define ERROR_MSG_NO_CONFIG   "no config file found - using default values\n"

//defaults
#define DEFAULT_RESOLUTION 320
#define DEFAULT_PPS          5
#define DEFAULT_WIND      0.0f
#define DEFAULT_GRAVITY 9.798f

//masks
#define MASK_RESOLUTION 0x08
#define MASK_PPS 0x04
#define MASK_GRAVITATION 0x02
#define MASK_WIND 0x01

//constants
#define PI 3.14159265
#define SCALE 10.0
#define GRAVITY_ANGLE 270.0f


#define OPEN_FILE_METHOD "wb"

#define BMP_FILEHEADER_OFFSET_BITS 54
#define BMP_FILEHEADER_RESERVED 0

#define BMP_INFOHEADER_SIZE 40
#define BMP_INFOHEADER_PLANES 1
#define BMP_INFOHEADER_BIT_COUNT 24
#define BMP_INFOHEADER_COMPRESSION 0
#define BMP_INFOHEADER_X_PELS_PER_METER 0
#define BMP_INFOHEADER_Y_PELS_PER_METER 0
#define BMP_INFOHEADER_COLOR_USED 0
#define BMP_INFOHEADER_COLOR_IMPORTANT 0

#define MIDDLE_CIRCLE_RADIUS 10


#define STRING_GRAVITATION    "gravitation"
#define STRING_WIND           "wind"
#define STRING_RESOLUTION     "resolution"
#define STRING_PPS            "pps"

#define COUNT_MAX_CONFIG_PARAMS 4

#define FLOAT_ZERO 0.0f

#define ARG_COUNT_MINIMUM 4
#define ARG_COUNT_MAXIMUM 5

#define COUNT_NUMBERS_GRAVITY 1
#define COUNT_NUMBERS_RESOLUTION 2
#define COUNT_NUMBERS_WIND 2
#define COUNT_NUMBERS_PPS 1


typedef struct _point_
{
  unsigned int x_;
  unsigned int y_;
} Point;

typedef struct _vector_
{
  double x_;
  double y_;
} Vector;

typedef Vector Distance;

typedef unsigned char Byte;

typedef struct _BitmapFileHeader_
{
  char type_[2];
  int size_;
  int reserved_;
  int offset_bits_;
} BitmapFileHeader;

typedef struct _BitmapInfoHeader_
{
  int size_;
  int width_;
  int height_;
  short planes_;
  short bit_count_;
  int compression_;
  int size_image_;
  int x_pels_per_meter_;
  int y_pels_per_meter_;
  int clr_used_;
  int clr_important_;
} BitmapInfoHeader;

typedef struct _Circle_
{
  int x_;
  int y_;
  int r_;
} Circle;

typedef struct _Color_
{
  int red_;
  int green_;
  int blue_;
} Color;

//-----------------------------------------------------------------------------
///
/// This method is for writing rgb data to a bitmap file
///
/// @param filename has to be a valid .bmp filename.
/// @param width is the resolution for the x-axis given in pixel.
/// @param height is the resolution for the y-axis given in pixel.
/// @param rgb is an array of bytes, first value is red, second green and the
///        third blue.
///
/// @return int for success or failure of the whole function
//
int write_bmp(const char *filename, int width, int height, Byte *rgb)
{
  int count_i;
  int count_j;
  int offset;
  int bytes_per_line;
  Byte *file_line;

  FILE *bmp_file;
  BitmapFileHeader bmp_file_header;
  BitmapInfoHeader bmp_info_header;

  bytes_per_line = ((3 * (width + 1)) / 4) * 4;

  // File type must be 4D43 hex for Bitmap
  bmp_file_header.type_[0] = 'B';
  bmp_file_header.type_[1] = 'M';
  bmp_file_header.offset_bits_ = BMP_FILEHEADER_OFFSET_BITS;
  bmp_file_header.size_ = bmp_file_header.offset_bits_ + 
    bytes_per_line * height;
  bmp_file_header.reserved_ = BMP_FILEHEADER_RESERVED;

  bmp_info_header.size_ = BMP_INFOHEADER_SIZE;
  bmp_info_header.width_ = width;
  bmp_info_header.height_ = height;
  bmp_info_header.planes_ = BMP_INFOHEADER_PLANES;
  bmp_info_header.bit_count_ = BMP_INFOHEADER_BIT_COUNT;
  bmp_info_header.compression_ = BMP_INFOHEADER_COMPRESSION;
  bmp_info_header.size_image_ = bytes_per_line * height;
  bmp_info_header.x_pels_per_meter_ = BMP_INFOHEADER_X_PELS_PER_METER;
  bmp_info_header.y_pels_per_meter_ = BMP_INFOHEADER_Y_PELS_PER_METER;
  bmp_info_header.clr_used_ = BMP_INFOHEADER_COLOR_USED;
  bmp_info_header.clr_important_ = BMP_INFOHEADER_COLOR_IMPORTANT;

  // Open filestream with write binary
  bmp_file = fopen(filename, OPEN_FILE_METHOD);
  if(bmp_file == NULL)
  {
    return ERROR_OPEN_FILE;
  }

  fwrite(&bmp_file_header.type_, 2, 1, bmp_file);
  fwrite(&bmp_file_header.size_, 4, 1, bmp_file);
  fwrite(&bmp_file_header.reserved_, 4, 1, bmp_file);
  fwrite(&bmp_file_header.offset_bits_, 4, 1, bmp_file);

  fwrite(&bmp_info_header.size_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.width_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.height_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.planes_, 2, 1, bmp_file);
  fwrite(&bmp_info_header.bit_count_, 2, 1, bmp_file);
  fwrite(&bmp_info_header.compression_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.size_image_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.x_pels_per_meter_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.y_pels_per_meter_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.clr_used_, 4, 1, bmp_file);
  fwrite(&bmp_info_header.clr_important_, 4, 1, bmp_file);

  file_line = malloc(bytes_per_line);
  if(file_line == NULL)
  {
    fclose(bmp_file);
    return ERROR_OUT_OF_MEMORY;
  }

  for(count_i = height - 1; count_i >= 0; count_i--)
  {
    // Change the rgb to bgr for bmp format
    for(count_j = 0; count_j < width; count_j++)
    {
      offset = count_i * width * 3 + count_j * 3;
      file_line[3 * count_j] = rgb[offset + 2];
      file_line[3 * count_j + 1] = rgb[offset + 1];
      file_line[3 * count_j + 2] = rgb[offset];
    }

    // Fill rest of the row with 0x0 to get the 4^x elements
    for(count_j = width * 3; count_j < bytes_per_line; count_j++)
    {
      file_line[count_j] = 0x0;
    }

    // Write line to file
    fwrite(file_line, bytes_per_line, 1, bmp_file);
  }
  free(file_line);
  fclose(bmp_file);

  return SUCCESS;
}

//-----------------------------------------------------------------------------
///
/// This method is for generating an bitmap image with some points representing
/// a line, a circle in the middle of the image and a background color.
///
/// @param points represents the line which has to be drawn onto the image.
/// @param width is the resolution for the x-axis given in pixel.
/// @param height is the resolution for the y-axis given in pixel.
/// @param filename has to be a valid .bmp filename.
///
/// @return int for success or failure of the whole function
//
int generateImage(short *points, int width, int height, 
                  char *filename)
{
  int count_i;
  int row;
  int col;
  int radius;

  // Color circle: dark green
  // Color background: green
  // Color line: white
  //                     R,   G,   B
  Color col_circle = {  39, 174,  96 };
  Color col_backgr = {  46, 204, 113 };
  Color col_line =   { 255, 255, 255 };

  Byte data[height * width * 3];

  Circle circle = { width / 2 - 1, height / 2 - 1, MIDDLE_CIRCLE_RADIUS };

  for(count_i = 0; count_i < height * width * 3; count_i += 3)
  {
    row = (count_i / 3) / width;
    col = (count_i / 3) % width;

    // Get radius with pythagoras
    radius = sqrt(pow(fabs(row - circle.y_), 2) + 
                  pow(fabs(col - circle.x_), 2));

    if(radius < circle.r_)
    {
      data[count_i] = col_circle.red_;
      data[count_i + 1] = col_circle.green_;
      data[count_i + 2] = col_circle.blue_;
    }
    else if(radius < circle.r_ + 1)
    {
      data[count_i] = (col_circle.red_ + col_backgr.red_) / 2;
      data[count_i + 1] = (col_circle.green_ + col_backgr.green_) / 2;
      data[count_i + 2] = (col_circle.blue_ + col_backgr.blue_) / 2;
    }
    else if (points[ARRAY_ACCESS(col, row, width)])
    {
      data[count_i] = col_line.red_;
      data[count_i + 1] = col_line.green_;
      data[count_i + 2] = col_line.blue_;
    }
    else
    {
      data[count_i] = col_backgr.red_;
      data[count_i + 1] = col_backgr.green_;
      data[count_i + 2] = col_backgr.blue_;
    }
  }

  return write_bmp(filename, width, height, data);
}

//------------------------------------------------------------------------------
///
/// Print the arguments
///
/// @param print_speed The value should be printed to Standard Output
/// @param print_angle The value should be printed to Standard Output
/// @param print_bit_map_name The value should be printed to Standard Output
///
/// @return void no Error checking needed
//
void printArguments(float *print_angle, float *print_speed, 
  char **print_bit_map_name)
{
  printf("%f, %f, %s\n", *print_angle, *print_speed, *print_bit_map_name);
}

//------------------------------------------------------------------------------
///
/// Checks if a string contains a '-', ',' or '.' to deem it not an unsigned int
///
/// @param str   The string to be checked
///
/// @return int   0 if the string can be converted.
//
int isUnsignedInteger(char* str)
{
  return strstr(str,"-") == NULL && strstr(str, ",") == NULL && 
    strstr(str, ".") == NULL;
}

//------------------------------------------------------------------------------
///
/// Parsing the buffer back to the lines
///
/// @param buf_to_parse Contains the whole config file
/// @param nr_gravity   Is the default value to be changed if necessary
/// @param res_width    Is the default value to be changed if necessary
/// @param res_height   Is the default value to be changed if necessary
/// @param nr_pps       Is the default value to be changed if necessary
/// @param wind_angle   Is the default value to be changed if necessary
/// @param wind_force   Is the default value to be changed if necessary
///
/// @return void no Error checking needed
//
void parseBuffer(char *buf_to_parse, float *nr_gravity, unsigned int *res_width, 
  unsigned int *res_height, unsigned int *nr_pps, float *wind_angle, 
  float *wind_force)
{
  char *mark_end = NULL;
  int count_max_config_params = COUNT_MAX_CONFIG_PARAMS; 
  int been_set = 0; // incorrect = 0;

  while((mark_end = strchr(buf_to_parse, '\n')) != NULL)
  {
    char *temp = NULL;
    *mark_end = '\0';
    temp = buf_to_parse;
    buf_to_parse = ++mark_end;
    char *pch;

    if (strlen(temp) == 0)
      continue;

    if((pch = strstr(temp, STRING_GRAVITATION)) != NULL)
    {
      if((sscanf(temp, "%*s %f", nr_gravity) == COUNT_NUMBERS_GRAVITY) &&
        (been_set & MASK_GRAVITATION) == 0)
      {
		    been_set |= MASK_GRAVITATION;
        printf("gravitation set to %.2f\n", *nr_gravity);
        count_max_config_params--;
      }
      else
      {
        //incorrect++;
        *nr_gravity = DEFAULT_GRAVITY;
      }
    }
    else if((pch = strstr(temp, STRING_WIND)) != NULL)
    {
      if(sscanf(temp, "%*s %f %f", wind_angle, wind_force) ==
        COUNT_NUMBERS_WIND && (been_set & MASK_WIND) == 0)
      {
        been_set |= MASK_WIND;
        printf("wind set to %.2f degrees with force %.2fm/s^2\n", *wind_angle, 
          *wind_force);
        count_max_config_params--;
      }
      else
      {
        *wind_angle = DEFAULT_WIND;
        *wind_force = DEFAULT_WIND;
        //incorrect++;        
      }
    }
    else if((pch = strstr(temp, STRING_RESOLUTION)) != NULL)
    {
      if(sscanf(temp, "%*s %u %u", res_width, res_height) == 
        COUNT_NUMBERS_RESOLUTION && isUnsignedInteger(temp) &&
        *res_width % 1 == 0 && *res_height % 1 == 0 && 
        (been_set & MASK_RESOLUTION) == 0)
      {
        been_set |= MASK_RESOLUTION;
        printf("resolution set to %u:%u\n", *res_width, *res_height);
        count_max_config_params--;
      }
      else
      {
        *res_width = DEFAULT_RESOLUTION;
        *res_height = DEFAULT_RESOLUTION;
        //incorrect++;
      }
    }
    else if((pch = strstr(temp, STRING_PPS)) != NULL)
    {      
      if (sscanf(temp, "%*s %u", nr_pps) == COUNT_NUMBERS_PPS &&
        isUnsignedInteger(temp) && (been_set & MASK_PPS) == 0)
      {
        been_set |= MASK_PPS;
        printf("pps set to %u\n", *nr_pps);
        count_max_config_params--;
      }
      else
      {
        *nr_pps = DEFAULT_PPS;
        //incorrect++;
      }
    }
    //else
    //  incorrect++;
  }
  if (/*incorrect +*/ count_max_config_params != 0)
    printf("%d " ERROR_MSG_CONFIG_FILE, /*incorrect +*/ count_max_config_params);
}


//------------------------------------------------------------------------------
///
/// opens the config.cfg file
///
/// @param config_file Getting the whole file in one string with whitespaces
///
/// @return ERROR_OUT_OF_MEMORY means that there is no RAM available anymore.
//
int openConfig(char config_file[], float *nr_gravity, unsigned int *res_width, 
  unsigned int *res_height, unsigned int *nr_pps, float *wind_angle, 
  float *wind_force)
{
  FILE *file_descriptor;
  char *buffer = NULL;
  int file_size = 0;

  file_descriptor = fopen(config_file, "r");
  if(file_descriptor == NULL)
  {
    printf(ERROR_MSG_NO_CONFIG);
    return SUCCESS;
  }
  else 
  {
    //getting file size
    fseek(file_descriptor, 0, SEEK_END);
    file_size = ftell(file_descriptor);
    rewind(file_descriptor);

    //allocate memory for the whole file:
    buffer = (char*) malloc (sizeof(char)*file_size);
    if(buffer == NULL)
    {
      free(buffer);
      return ERROR_OUT_OF_MEMORY;
    }
    while(!feof(file_descriptor))
    {
      fread(buffer, 1, file_size, file_descriptor);
      if(ferror(file_descriptor))
      {
        fclose(file_descriptor);
        free(buffer);
        return ERROR_ARGUMENTS;
      }
    }

    buffer = (char *)realloc(buffer, file_size + 2);
    if (buffer == NULL)
    {
      return ERROR_OUT_OF_MEMORY;
    }
    buffer[file_size] = '\n';
    buffer[file_size + 1] = '\0';

    parseBuffer(buffer, nr_gravity, res_width, res_height, nr_pps, wind_angle, 
    wind_force);
  }
  
  free(buffer);
  fclose(file_descriptor);
  return SUCCESS;
}


//------------------------------------------------------------------------------
///
/// checks the file ending of the bitmap file
///
/// @param bit It is the string given by the function readArcuments()
///
/// @return SUCCESS if the ending exists
/// @return ERROR_ARGUMENTS if the ending does not exist
//
int checkBitmapName(char **bit)
{
  char *found_ending;
  found_ending = strstr(*bit, ".bmp");
  if (found_ending == NULL)
  {
    return ERROR_ARGUMENTS;
  }
  return SUCCESS;
}


//------------------------------------------------------------------------------
///
/// read argmunents from the commandline
///
/// @param arg_count It is the number of arguments
/// @param arg_value It is used to get the characters of the arguments
/// @param arg_angle A global will be need an, in case, overwritten
/// @param arg_speed A global will be need an, in case, overwritten
/// @param arg_bit_name It will be need to compare, if the file ending exists.
///
/// @return error It depends on the values, which will be returned by two other
/// functions: checkIsFloat() and checkBitmapName()
//
int readArguments(int arg_count, char **arg_value, float *arg_angle, 
  float *arg_speed, char **arg_bitmap_name, float *nr_gravity, 
  unsigned int *res_width,   unsigned int *res_height, unsigned int *nr_pps, 
  float *wind_angle,   float *wind_force)
{
  int arg_iterator;
  int error = 0;
     
  for(arg_iterator = 1; arg_iterator < arg_count; arg_iterator++)
  {
    if(arg_iterator == 1)
    {
      if (sscanf(arg_value[arg_iterator], "%f", arg_angle) == 0)
      {
        return ERROR_ARGUMENTS;
      }
      /**arg_angle = (float)atof(arg_value[arg_iterator]);
      if((error = checkIsFloat(arg_value[arg_iterator])) != 0)
      {
        return error;
      }*/
    }
    else if(arg_iterator == 2)
    {
      if(sscanf(arg_value[arg_iterator], "%f", arg_speed) == 0)
      {
        //*arg_speed = (float)atof(arg_value[arg_iterator]);
        return ERROR_ARGUMENTS;
      }
      if (*arg_speed <= 0)
      {
        return ERROR_SPEED;
      }
      /*
      if((error = checkIsFloat(arg_value[arg_iterator])) != 0)
      {
        return error;
      }*/
    }

    else if(arg_iterator == 3)
    {
      *arg_bitmap_name = arg_value[arg_iterator];
      if((error = checkBitmapName(arg_bitmap_name)) != 0)
      {
        return error;
      }
    }
    else if(arg_iterator == 4)
    {
      if((error = openConfig(arg_value[arg_iterator], nr_gravity, res_width, 
        res_height, nr_pps, wind_angle, wind_force)) != 0)
      {
        return error;
      }
    }
  }

  if(arg_count == ARG_COUNT_MINIMUM)
  {
    printf(ERROR_MSG_NO_CONFIG);
  }
  return SUCCESS;
}


//------------------------------------------------------------------------------
///
/// check the arguments count
///
/// @param counter It will be tested if the counter is in a certain range. 
///
/// @return as always SUCCESS, else ERROR_ARGUMENTS
//
int checkArgumentsCount(int counter)
{
  if(counter < ARG_COUNT_MINIMUM || counter > ARG_COUNT_MAXIMUM)
  {
    return ERROR_ARGUMENTS;
  }
  return SUCCESS;
}


//------------------------------------------------------------------------------
///
/// Printf the error messages
///
/// @param error It is a number, given by the other returns and it is depending 
/// on it, which messages will be printed on the stdout.
///
/// @return SUCCESS is set as default
/// @return ERROR_ARGUMENTS     return 1, if sth is wrong with the arguments
/// @return ERROR_OUT_OF_MEMORY return 2, if sth is wrong with reading the *cfg
/// @return ERROR_OPEN_FILE    return 3, if sth is wrong with creating the bmp
/// @return ERROR_SPEED         return 4, if sth is wrong with the speed. 
//
int errorHandling(int error)
{
  switch(error)
  {
    case 1: 
      printf(ERROR_MSG_ARG);
      return ERROR_ARGUMENTS;
    case 2:
      printf(ERROR_MSG_OUT_OF_MEM);
      return ERROR_OUT_OF_MEMORY;
    case 3:
      printf(ERROR_MSG_OPEN_FILE);
      return ERROR_OPEN_FILE;
    case 4:
      printf(ERROR_MSG_SPEED);
      return ERROR_SPEED;

    default: 
      return SUCCESS;
  }
}

//-----------------------------------------------------------------------------
///
/// Calculates the vector based on a value and an angle
///
/// @param value      The value of the force of the to be vector
/// @param angle      The direction of the force
///
/// @return Vector    A vector with a certain force in x and y direction.
//
Vector getVector(float angle, float value)
{
  Vector vector = { 0, 0 };
  double radian = angle * PI / 180.0f;
  vector.x_ = (value * cos(radian)) / SCALE;
  vector.y_ = (-value * sin(radian)) / SCALE;
  return vector;
}


//-----------------------------------------------------------------------------
///
/// Calculates the delta distance based on a vector and the passed time
///
/// @param vector     The force that is applied over time in a certain
///                   direction
/// @param time       The time that has passed so far
///
/// @return Distance  The delta distance after a certain time
//
Distance getDistance(Vector vector, double time)
{
  Distance distance = { 0, 0 };
  distance.x_ = vector.x_ * time;
  distance.y_ = vector.y_ * time;
  return distance;
}


//-----------------------------------------------------------------------------
///
/// Calculates the current point based on a starting point and the 3 distances
/// applied from speed, gravity and wind over time 
///
/// @param center   The starting point. By default the center of the image.
/// @param delta_v  The distance due to the speed after t seconds.
/// @param delta_g  The distance due to the gravity after t seconds.
/// @param delta_w  The distance due to the wind after t seconds.
///
/// @return Point   The point the projectile is currently at
//
Point getCurrentPoint(Point center, Distance delta_v, Distance delta_g,
  Distance delta_w)
{
  Point final = { 0, 0 };
  final.x_ = center.x_ + delta_v.x_ + delta_g.x_ + delta_w.x_;
  final.y_ = center.y_ + delta_v.y_ + delta_g.y_ + delta_w.y_;
  return final;
}

//-----------------------------------------------------------------------------
///
/// Calculates the individual points of the projectile's flightpath
///
/// @param points   A pointer to the array of point structure which's x_ und y_
///                 coordinates contain the single calculated dots.
///                 The array has to be freed later!!!
/// @param *length  The length of the calculated array
/// @param width    The width of the image
/// @param height   The height of the image
/// @param pps      How many points to calculate for each simulated second
/// @param angle    The angle at which the projectile is shot
/// @param speed    The speed at which the projectile is shot
/// @param gravity  Gravity to pull the projectile down
/// @param wind_angle         The angle of the wind
/// @param wind_acceleration  The force of the wind
///
/// @return int     SUCCESS if successfully executed
///                 ERROR_OUT_OF_MEMORY if no memory could be allocated              
//
int getPoints(Point **points, int *length, unsigned int width, 
  unsigned int height, unsigned int pps, float angle, float speed,
  float gravity, float wind_angle, float wind_acceleration)
{

  Vector vec_speed = getVector(angle, speed);
  Vector vec_grav = getVector(GRAVITY_ANGLE, gravity);
  Vector vec_wind = getVector(wind_angle, wind_acceleration);

  Point current_point = { width / 2, height / 2 };
  Point center = { width / 2, height / 2 };

  Distance dis_speed = { 0, 0 };
  Distance dis_grav = { 0, 0 };
  Distance dis_wind = { 0, 0 };

  double delta_t = 1.0 / pps;
  double time = delta_t;

  free(*points);
  *points = malloc(sizeof(Point));

  if((*points) == NULL)
    return  ERROR_OUT_OF_MEMORY;

#ifdef DEBUG
  printf("vector v = [%f,%f]\n", vec_speed.x_, vec_speed.y_);
  printf("vector g = [%f,%f]\n", vec_grav.x_, vec_grav.y_);
  printf("vector w = [%f,%f]\n", vec_wind.x_, vec_wind.y_);
#endif

  for((*length) = 0; current_point.x_ < width && current_point.y_ < height;
    time += delta_t, (*length)++)
  {  

#ifdef DEBUG
  printf("%d. loop: time = %f\n\t\t\ttime^2 = %f\n", *length,
    time, time * time);
#endif
  
    (*points)[*length] = current_point;
    (*points) = realloc(*points, ((*length) + 2) * sizeof(Point));
    if((*points) == NULL)
      return  ERROR_OUT_OF_MEMORY;

    dis_speed = getDistance(vec_speed, time);
    dis_grav = getDistance(vec_grav, (time * time) / 2.0);
    dis_wind = getDistance(vec_wind, (time * time) / 2.0);

#ifdef DEBUG
  printf("\t\t\tdistance v = [%f,%f]\n", dis_speed.x_, dis_speed.y_);
  printf("\t\t\tdistance g = [%f,%f]\n", dis_grav.x_, dis_grav.y_);
  printf("\t\t\tdistance w = [%f,%f]\n", dis_wind.x_, dis_wind.y_);
#endif

    current_point = getCurrentPoint(center, dis_speed, dis_grav, dis_wind);
  }
  
  *points = realloc(*points, ((*length) + 1) * sizeof(Point));
  
  if((*points) == NULL)
    return  ERROR_OUT_OF_MEMORY;
          
  (*points)[*length] = current_point;
  (*length)++;

#ifdef DEBUG
  for(pps = 0; pps < *length; pps++)
    printf("\t%d. Point: {%d,%d}\n", pps, (*points)[pps].x_, (*points)[pps].y_);
#endif

  return SUCCESS;
}


//-----------------------------------------------------------------------------
///
/// Sign function that returns either 1, 0 or -1. Basically the sign of the
/// input. 
///
/// @param value    The value of which the sign should be estimated  
///
/// @return int     1 if the value is positive
///                 0 if the value is zero
///                 -1 if the value is negative
//
int sign(int value)
{
  return (value > 0) ? 1 : (value < 0) ? -1 : 0;
}

//-----------------------------------------------------------------------------
///
/// Calculates an array of pixels connecting the dots calculated before with
/// lines using the Bresenham algorithmus.
/// http://de.wikipedia.org/wiki/Bresenham-Algorithmus
///
/// @param pixel    An array with the lendth of width * height. A value set to 
///                 1 indicates that the pixel is set, 0 is clear.
///                 The array can be accessed with x and y coordinates using
///                 the ARRAY_ACCESS(x, y, width) macro.
/// @param points   An array of Point containing the calculated points of the
///                 flightpath. The last point in the array is already beyond
///                 the width/height limit.
/// @param length   The length of the Point array
/// @param width    The width of the image
/// @param height   The height of the image
///
/// @return int     SUCCESS if successfully executed
///                 ERROR_OUT_OF_MEMORY if no memory could be allocated
//
int getPixel(short **pixel, Point *points, int length, int width, int height)
{
  int count_points = 0;
  int x = 0;
  int y = 0;
  int count_pixel = 0;
  int delta_x = 0;
  int delta_y = 0;
  int increase_x = 0;
  int increase_y = 0;
  int parallel_step_x = 0;
  int parallel_step_y = 0;
  int diagonal_step_x = 0;
  int diagonal_step_y = 0;
  int error_short = 0;
  int error_long = 0;
  int error_total = 0;
    
  if(points == NULL)
    return ERROR_OUT_OF_MEMORY;
  
  free(*pixel);
  *pixel = malloc(width * height * sizeof(short*));
      
  if((*pixel) == NULL)
    return ERROR_OUT_OF_MEMORY;
    
  for(x = 0; x < width; x++)
    for(y = 0; y < height; y++)
      (*pixel)[ARRAY_ACCESS(x, y, width)] = 0;
   
  //the Bresenham algorithmus used on two points from the array
  //http://de.wikipedia.org/wiki/Bresenham-Algorithmus
  for(count_points = 0; count_points + 1 < length; count_points++)
  {
    // Get the distance between the two points
    delta_x = points[count_points + 1].x_ - points[count_points].x_;
    delta_y = points[count_points + 1].y_ - points[count_points].y_;
 
    //which direction the line goes (to increase the counters to)
    increase_x = sign(delta_x);
    increase_y = sign(delta_y);
    delta_x = abs(delta_x);
    delta_y = abs(delta_y);

    if (delta_x > delta_y)
    {
      // x is faster
      parallel_step_x = increase_x;
      parallel_step_y = 0;    
      diagonal_step_x = increase_x; 
      diagonal_step_y = increase_y;
      error_short = delta_y;   
      error_long = delta_x;
    }
    else
    {
      // y is faster
      parallel_step_x = 0;    
      parallel_step_y = increase_y;
      diagonal_step_x = increase_x; 
      diagonal_step_y = increase_y;
      error_short = delta_x;   
      error_long = delta_y;
    }

    //initialisation for loop
    x = points[count_points].x_;
    y = points[count_points].y_;
    error_total = error_long / 2;
    (*pixel)[ARRAY_ACCESS(x, y, width)] = 1;

    for(count_pixel = 0; count_pixel < error_long; ++count_pixel)
    {
      error_total -= error_short;
      if(error_total < 0)
      {
        //time for error step
        error_total += error_long;
        x += diagonal_step_x;
        y += diagonal_step_y;
      }
      else
      {
        x += parallel_step_x;
        y += parallel_step_y;
      }
      
      if(x < 0 || x > width || y < 0 || y > height)
        break;        
      (*pixel)[ARRAY_ACCESS(x, y, width)] = 1;
    }
    
#if DEBUG
   printf("%d. line connected from [%d,%d] to [%d,%d]\n", 
     count_points, points[count_points].x_, points[count_points].y_,
     points[count_points + 1].x_, points[count_points + 1].y_);
#endif
  }     
  return SUCCESS;
}


int main(int argc, char **argv)
{
  //set through input para
  float speed = 0.0f;
  float angle = 0.0f;
  char *bitmap_name = NULL;
  
  //default values
  unsigned int width =  DEFAULT_RESOLUTION;
  unsigned int height = DEFAULT_RESOLUTION;
  unsigned int pps = DEFAULT_PPS;
  float gravity = DEFAULT_GRAVITY;
  float wind_angle = DEFAULT_WIND;
  float wind_force = DEFAULT_WIND;

  int error = 0;  
  
  int length = 0;
  Point *points = NULL;  
  short *pixel = NULL;

  error = checkArgumentsCount(argc);
  CHECK_ERROR(error, points, pixel);

  error = readArguments(argc, argv, &angle, &speed, &bitmap_name, 
    &gravity, &width, &height, &pps, &wind_angle, &wind_force);
  CHECK_ERROR(error, points, pixel);
  

#ifdef DEBUG   
  printArguments(&angle, &speed, &bitmap_name);
#endif
  //---------------------------------------------

  error = getPoints(&points, &length, width, height, pps, angle, speed, gravity,
    wind_angle, wind_force);   
  CHECK_ERROR(error, points, pixel);
    
  error = getPixel(&pixel, points, length, width, height);
  CHECK_ERROR(error, points, pixel);
  
  //---------------------------------------------

  error = generateImage(pixel, width, height, bitmap_name);
  CHECK_ERROR(error, points, pixel);
  
  
  free(points);
  free(pixel);

  return SUCCESS;
}

