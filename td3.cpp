#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* array2=new double[new_size];
    for (int i=0; i<new_size;i++){
        if (i<length){
            array2[i]=array[i];
        }
        else{
            array2[i]=0;
        }
  }
    delete array;
    return array2;

//return NULL; // YOU CAN CHANGE THIS
}

double* shrink_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* array2=new double[new_size];
    for (int i=0; i<new_size;i++){
            array2[i]=array[i];
        }

    delete array;
    return array2;

  return NULL; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    if (current_size==max_size){
        double*  array2=extend_array(array, current_size+1,max_size+5);
        array2[current_size]=element;
        current_size+=1;
        max_size+=5;
        return array2; // YOU CAN CHANGE THIS
    }
    double* array2=extend_array(array, current_size,current_size+1);
    array2[current_size]=element;
    current_size+=1;
    return array2;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    current_size-=1;
    if (max_size-current_size>=5){
        array=shrink_array(array, max_size,current_size);
        max_size=current_size;
         // YOU CAN CHANGE THIS
    }
    //double* array2=shrink_array(array, current_size,current_size-1);
    return array;

    //return NULL; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;
  telemetry=append_to_array(t, telemetry, telemetry_current_size,telemetry_max_size);
  telemetry=append_to_array(x, telemetry, telemetry_current_size,telemetry_max_size);
  telemetry=append_to_array(y, telemetry, telemetry_current_size,telemetry_max_size);
  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;

      y = v0_y * t  - 0.5 * g * t * t;

      x = v0_x * t;
      if (y >= 0 && (! hit_target) && (! hit_obstacle)){
          telemetry=append_to_array(t, telemetry, telemetry_current_size,telemetry_max_size);
          telemetry=append_to_array(x, telemetry, telemetry_current_size,telemetry_max_size);
          telemetry=append_to_array(y, telemetry, telemetry_current_size,telemetry_max_size);

      }


    }
  }

  return hit_target;
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
    // IMPLEMENT YOUR FUNCTION HERE
    //double times[tot_telemetries*telemetries_sizes/3];
    /*for (int i=0; i<tot_telemetries; i++){
        for (int j=0; j<telemetries_sizes[i];j++){
            global_telemetry=append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
        }}*/
    /*for (int k=0; k<global_telemetry_current_size-5;k+=3){
            if (global_telemetry[k]>global_telemetry[k+3]){
          global_telemetry[k],global_telemetry[k+3]=global_telemetry[k+3],global_telemetry[k];
          global_telemetry[k+1],global_telemetry[k+4]=global_telemetry[k+4],global_telemetry[k+1];
          global_telemetry[k+2],global_telemetry[k+5]=global_telemetry[k+5],global_telemetry[k+2];
            }
    }*/
  //Update size
    /*for (int i = 0; i < tot_telemetries; i++) {
        global_telemetry_max_size += telemetries_sizes[i];
    }

    // Allocate memory for global_telemetry
    global_telemetry = new double[global_telemetry_max_size];
    global_telemetry_current_size = 0;*/

    for (int i=0; i<tot_telemetries; i++){
        for (int j=0; j<telemetries_sizes[i];j++){
            global_telemetry=append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
        }}

    for (int i = 0; i < global_telemetry_current_size-3; i+=3) {
        for (int j = 0; j < global_telemetry_current_size - i - 5; j+=3) {
      // Compare the t coordinates of adjacent targets
            if (global_telemetry[j] > global_telemetry[j + 3]) {
                double temp_t = global_telemetry[j];
                global_telemetry[j] =  global_telemetry[j + 3];
                global_telemetry[j + 3] = temp_t;

                double temp_x = global_telemetry[j + 1];
                global_telemetry[j + 1] = global_telemetry[j+4];
                global_telemetry[j+4] = temp_x;

                double temp_y = global_telemetry[j + 2];
                global_telemetry[j + 2] = global_telemetry[j+5];
                global_telemetry[j+5] = temp_y;
                //delete temp_t,temp_x,temp_y;
      }
    }
    }

}












