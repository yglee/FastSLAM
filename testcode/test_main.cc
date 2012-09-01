#include <iostream>

#include "openCLWrapper.h"
#include <assert.h>

#define PROGRAM_FILE "vector_add_gpu.cl"

using namespace std;

int main()
{
    
    cl_int error = 0;   // Used to handle error codes
    cl_platform_id platform;
    cl_context context;
    cl_command_queue queue;
    cl_device_id device;
    
    
    // Platform
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
    const int size =12345667;
    float *src_a_h = new float[size];
    float *src_b_h = new float[size];
    float *res_h = new float[size];
    
    //initialize both vectors
    for (int i=0; i< size; i++) {
        src_a_h[i] = src_b_h[i] = (float) i;
    }
    
    const int mem_size = sizeof(float)*size;
    //allocates a buffer of size mem_size and copies mem_size bytes from src_a_h
    cl_mem src_a_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_a_h, &error);
    cl_mem src_b_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_b_h, &error);
    cl_mem res_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_size, NULL, &error);
    
    //creates the program
    size_t src_size=0;
    
    //get kernel path
    string kernelpaths = "/Users/ylee8/FastSLAM/testcode"; //BuildRegGetPackageLocation( "FastSLAM" ) + "/testcode/";
    
    //get cl_program
    cl_program program = context.CreateProgramFromFile(kernelpaths + PROGRAM_FILE); 
    
    
    //build the program
    error = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    assert(error == CL_SUCCESS);
    
    //shows the log
    char* build_log;
    size_t log_size;
    
    //First call to know the proper size
    
    
}

string BuildRegGetPackageLocation(string packagename) {

}
