#include <iostream>

#include "openCLWrapper.h"

using namespace std;

int main()
{
    
    cl_int error = 0;   // Used to handle error codes
    cl_platform_id platform;
    cl_context context;
    cl_command_queue queue;
    cl_device_id device;
    
    
    // Platform
    //error = oclGetPlatformID(&platform);
    error = clGetPlatformIDs(1, &platform, 0);
    
    if (error != CL_SUCCESS) {
        cout << "Error getting platform id: " <<error << endl;
        exit(error);
    }
    // Device
    error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if (error != CL_SUCCESS) {
        cout << "Error getting device ids: " << error << endl;
        exit(error);
    }
    // Context
    context = clCreateContext(0, 1, &device, NULL, NULL, &error);
    if (error != CL_SUCCESS) {
        cout << "Error creating context: " << error << endl;
        exit(error);
    }
    // Command-queue
    queue = clCreateCommandQueue(context, device, 0, &error);
    if (error != CL_SUCCESS) {
        cout << "Error creating command queue: " << error << endl;
        exit(error);
    }
     
    //allocate memory
    const int mem_size = sizeof(float)*size;
    
}
