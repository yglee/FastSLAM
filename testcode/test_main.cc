
int main()
{
/*
	// Returns the error code
	cl_int oclGetPlatformID (cl_platform_id *platforms)   // Pointer to the platform object

	// Returns the error code
	cl_int clGetDeviceIDs  (cl_platform_id platform,
           cl_device_type device_type,   // Bitfield identifying the type. For the GPU we use CL_DEVICE_TYPE_GPU
           cl_uint num_entries,   // Number of devices, typically 1
           cl_device_id *devices,   // Pointer to the device object
           cl_uint *num_devices)   // Puts here the number of devices matching the device_type

	// Returs the context
	cl_context clCreateContext (const cl_context_properties *properties,   // Bitwise with the properties (see specification)
                cl_uint num_devices,   // Number of devices
                const cl_device_id *devices,   // Pointer to the devices object
                void (*pfn_notify)(const char *errinfo, const void *private_info, size_t cb, void *user_data),   // (don't worry about this)
                void *user_data,   // (don't worry about this)
                cl_int *errcode_ret)   // error code result
*/
}
