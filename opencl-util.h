#pragma once
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include <typeinfo>

bool ext_list_contains_ext(std::string ext, std::string str) {
	size_t old_pos = 0;
	size_t space_pos = str.find(' ', old_pos); // extensions string is space delimited
	while (space_pos != str.npos)
	{
		if (strcmp(ext.c_str(), str.substr(old_pos, space_pos - old_pos).c_str()) == 0)
		{
			return true;
		}
		do
		{
			old_pos = space_pos + 1;
			space_pos = str.find(' ', old_pos);
		} while (space_pos == old_pos);
	}
	return false;
}

const char* getErrorString(cl_int error)
{
	switch (error) {
		// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}

static std::string convertToGoodNumber(double nBytes) {
	std::vector<std::string> prefixes{"b", "KB", "MB", "GB", "TB"};
	int n = 0;
	while (nBytes / 1024 > 0.6 && n < prefixes.size() - 1) {
		n++;
		nBytes /= 1024;
	}
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << nBytes;

	
	return stream.str() + " " + prefixes[n];
}

static std::string read_file(const char* fileName) {
	std::fstream f;
	f.open(fileName, std::ios_base::in);

	std::string res;
	while (!f.eof()) {
		char c;
		f.get(c);
		res += c;
	}

	f.close();

	res.pop_back();

	return std::move(res);
}

void assert_cl_success(const cl_int err, const char* message) {
	if (err) {
		std::string error_message = std::string(message);
		error_message += ".\n\tError Code: ";

		//throw std::runtime_error(error_message);
		std::cout << message << "\n\tError Code: " << err << "\n\tError Name: " << getErrorString(err) << "\n";
		exit(1);
	}
}

struct CL_Attribute {
	std::string name;
	void* raw_value;

	CL_Attribute() {
		name = "";
		raw_value = 0;
	}

	CL_Attribute(const char* object_type, void* object_id, std::string name, int attr_id): name(name) {
		size_t info_size;
		cl_int err;
		raw_value = NULL;
		if (strcmp(object_type, "platform") == 0) {
			err = clGetPlatformInfo((cl_platform_id)object_id, attr_id, 0, NULL, &info_size);
			assert_cl_success(err, "Error getting OpenCL attribute.");
			raw_value = malloc(info_size);
			clGetPlatformInfo((cl_platform_id)object_id, attr_id, info_size, raw_value, NULL);

		}
		else {
			err = clGetDeviceInfo((cl_device_id)object_id, attr_id, 0, NULL, &info_size);
			assert_cl_success(err, "Error getting OpenCL attribute.");
			raw_value = malloc(info_size);
			clGetDeviceInfo((cl_device_id)object_id, attr_id, info_size, raw_value, NULL);
		}
	}

	char* char_value() {
		return (char*)raw_value;
	}

	template <typename T>
	T value() {
		if (std::string(typeid(T).name()).find("char *") != std::string::npos)
			return char_value();
		return *(T*)raw_value;
	}

};

class CL_Device {
public:
	CL_Device(cl_device_id id): id(id) {
		for (int i = 0; i < (int)attr_names.size(); i++) {
			attributes.push_back(CL_Attribute("device", id, attr_names[i], attr_types[i]));
		}
	}

	cl_device_id get_id() {
		return id;
	}

	std::vector<CL_Attribute> get_attributes() {
		return attributes;
	}

	int get_num_attributes() {
		return (int)attributes.size();
	}

	template <typename T> 
	T find_attribute(std::string keyword) {
		for (int i = 0; i < attr_names.size(); i++) {
			if (attr_names[i].find(keyword) != std::string::npos) {
				return attributes[i].value<T>();
			}
		}
		return NULL;
	}

	template <typename T>
	std::string attribute_desc(std::string keyword) {
		for (int i = 0; i < attr_names.size(); i++) {
			if (attr_names[i].find(keyword) != std::string::npos) {
				return attr_names[i] + ": " + std::string(attributes[i].value<T>());
			}
		}
		return "Did not find: " + keyword + " in attributes.";
	}

	bool supports_extension(std::string ext) {
		size_t extensions_size;
		err = clGetDeviceInfo(id, CL_DEVICE_EXTENSIONS, 0, NULL, & extensions_size);
		assert_cl_success(err, "Error getting device extensions");
		if (extensions_size > 0)
		{
			char* extensions = (char*)malloc(extensions_size);
			err = clGetDeviceInfo(id, CL_DEVICE_EXTENSIONS, extensions_size, extensions, &extensions_size);
			if (!extensions)
				return false;
			return ext_list_contains_ext(ext, extensions);
		}
		return false;
	}

private:
	std::vector<CL_Attribute> attributes;

	cl_device_id id;
	cl_uint err = 0;

	std::vector<std::string> attr_names{ "Name", "Device Version", "Driver Version", "Parallel Computing Units", "Max Clock Frequency", "Max Constant Buffer Size"};
	std::vector<cl_device_info> attr_types{ CL_DEVICE_NAME, CL_DEVICE_VERSION,
		CL_DRIVER_VERSION, CL_DEVICE_MAX_COMPUTE_UNITS, CL_DEVICE_MAX_CLOCK_FREQUENCY, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE };
};

class CL_Platform {
public:

	CL_Platform(cl_platform_id id):id(id) {
		for (int i = 0; i < (int)attr_names.size(); i++) {
			attributes.push_back(CL_Attribute("platform", id, attr_names[i], attr_types[i]));
		}
		_get_devices();
	}
	
	cl_platform_id get_id() {
		return id;
	}

	std::vector<CL_Device> get_devices() {
		return devices;
	}

	int get_num_devices() {
		return (int)devices.size();
	}

	std::vector<CL_Attribute> get_attributes() {
		return attributes;
	}

	int get_num_attributes() {
		return (int)attributes.size();
	}

	template <typename T>
	T find_attribute(std::string keyword) {
		for (int i = 0; i < attr_names.size(); i++) {
			if (attr_names[i].find(keyword) != std::string::npos) {
				return attributes[i].value<T>();
			}
		}
		return NULL;
	}

	template <typename T>
	std::string attribute_desc(std::string keyword) {
		for (int i = 0; i < attr_names.size(); i++) {
			if (attr_names[i].find(keyword) != std::string::npos) {
				return attr_names[i] + ": " + std::string(attributes[i].value<T>());
			}
		}
		return "Did not find: " + keyword + " in attributes.";
	}

	bool supports_extension(std::string ext) {
		size_t extensions_size;
		err = clGetPlatformInfo(id, CL_PLATFORM_EXTENSIONS, 0, NULL, &extensions_size);
		assert_cl_success(err, "Error getting platform extensions");
		if (extensions_size > 0)
		{
			char* extensions = (char*)malloc(extensions_size);
			err = clGetPlatformInfo(id, CL_PLATFORM_EXTENSIONS, extensions_size, extensions, &extensions_size);
			if (!extensions)
				return false;
			return ext_list_contains_ext(ext, extensions);
		}
		return false;
	}

private:
	std::vector<CL_Attribute> attributes;
	cl_platform_id id;
	std::vector<CL_Device> devices;
	cl_uint err;

	std::vector<std::string> attr_names{ "Name", "Vendor", "Version", "Profile", "Extensions" };
	std::vector<cl_platform_info> attr_types{ CL_PLATFORM_NAME, CL_PLATFORM_VENDOR,
		CL_PLATFORM_VERSION, CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS };

	cl_uint _get_num_devices() {
		cl_uint num_devices;

		err = clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
		assert_cl_success(err, "Error getting number of OpenCL platform devices");
		return num_devices;
	}

	void _get_devices() {
		cl_uint num_devices = _get_num_devices();
		cl_device_id* the_devices = (cl_device_id*)malloc(sizeof(cl_device_id) * num_devices);

		err = clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, num_devices, the_devices, NULL);
		assert_cl_success(err, "Error getting OpenCL device IDs");
		
		if (!the_devices)
			return;
		for (int i = 0; i < (int)num_devices; i++) {
			devices.push_back(CL_Device(the_devices[i]));
		}
	}
};

class CL_Hardware_Info {
public:
	cl_int err;
	cl_device_id device_to_use;

	CL_Hardware_Info() {
		err = 0;
		device_to_use = NULL;
		required_extensions = {};
		num_platforms = 0;
		platforms = {};
		devices = {};
	}

	CL_Hardware_Info(std::vector<std::string> required_extensions): required_extensions(required_extensions) {
		init();
	}

private:
	std::vector<std::string> required_extensions;
	cl_uint num_platforms;
	std::vector<CL_Platform> platforms;
	std::vector<cl_device_id*> devices;

	void init() {
		cl_platform_id* the_platforms = _get_platforms();
		num_platforms = _get_num_platforms();
		for (int i = 0; i < (int)num_platforms; i++) {
			platforms.push_back(CL_Platform(the_platforms[i]));
		}
		_set_device_to_use();
		//device_to_use = platforms[1].get_devices()[0].get_id();
	}

	void _set_device_to_use() {
		std::vector<CL_Platform> valid_platforms = platforms;
		for (auto& extension : required_extensions) {
			for (int i = 0; i < platforms.size(); i++) {
				if (!valid_platforms[i].supports_extension(extension)) {
					valid_platforms.erase(valid_platforms.begin() + i);
					break;
				}
			}
			if (valid_platforms.size() == 0) {
				std::cout << "Error! No platforms support the extension: " << extension;
				exit(1);
			}
		}
		std::vector<CL_Device> valid_devices;
		for (auto& platform : valid_platforms) {
			std::vector<CL_Device> platform_devices = platform.get_devices();
			valid_devices.insert(std::end(valid_devices), std::begin(platform_devices), std::end(platform_devices));
		}
		for (auto& extension : required_extensions) {
			for (int i = 0; i < valid_devices.size(); i++) {
				if (!valid_devices[i].supports_extension(extension)) {
					valid_devices.erase(valid_devices.begin() + i);
					break;
				}
			}
			if (valid_devices.size() == 0) {
				std::cout << "Error! No devices support the extension: " << extension;
				exit(1);
			}
		}
		CL_Device chosen_device = valid_devices[0];
		device_to_use = chosen_device.get_id();
		std::cout << "Chosen Device: " << chosen_device.attribute_desc<char*>("Name") << std::endl;
	}

	cl_uint _get_num_platforms() {
		cl_uint num_platforms;
		err = clGetPlatformIDs(0, NULL, &num_platforms);
		assert_cl_success(err, "Error getting number of OpenCL platforms");

		return num_platforms;
	}

	cl_platform_id* _get_platforms() {
		cl_uint num_platforms = _get_num_platforms();
		cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platforms);

		err = clGetPlatformIDs(num_platforms, platforms, NULL);
		assert_cl_success(err, "Error getting OpenCL platform IDs");
		return platforms;
	}
};

struct CL_Buffer {
	cl_mem buffer;
	size_t size;

	CL_Buffer(cl_mem buffer, size_t size) : buffer(buffer), size(size) {};
};

class CL_Util {
public:
	CL_Util() {
		init();
	}

	CL_Util(const char* filename, std::vector <std::string> extensions = {}) : required_device_extensions(extensions) {
		init();

		create_and_build_program(filename);
	}

	CL_Util(const char* filename, const char* function_name, std::vector <std::string> extensions = {}) : required_device_extensions(extensions) {
		init();

		create_program_and_kernel(filename, function_name);
	}

	CL_Util(const char* filename, const char* function_name, size_t global_size, size_t local_size, std::vector <std::string> extensions = {}) : required_device_extensions(extensions) {
		init();

		global_item_size = global_size;
		local_item_size = local_size;


		create_program_and_kernel(filename, function_name);
	}

	void set_context_properties(cl_context_properties* cl_ctx_props) {
		cl_context_props = cl_ctx_props;
		_create_context();
		_create_command_queue();
		//init();
	}

	void set_options() {

	}

	// Getter methods

	CL_Hardware_Info   get_hardware_info() { return hardware_info; }
	cl_device_id     get_device_id()   { return device_id;   }
	cl_context       get_context()     { return context;     }
	cl_command_queue get_queue()       { return queue;       }
	cl_program       get_program()     { return program;     }
	cl_kernel        get_kernel()      { return kernel;      }
	size_t get_global_item_size() { return global_item_size;   }
	size_t get_local_item_size()  { return local_item_size;    }
	int get_current_kernel_arg()  { return current_kernel_arg; }

	void set_device(cl_device_id id) {
		hardware_info.device_to_use = id;
	}

	void create_program(const char* filename) {
		program = _create_program(filename);
	}

	void build_program() {
		_build_program();
	}

	void create_and_build_program(const char* filename) {
		program = _create_program(filename);
		_build_program();
	}

	void create_kernel(const char* function_name) {
		kernel = _create_kernel(function_name);
	}

	void create_program_and_kernel(const char* filename, const char* function_name) {
		create_and_build_program(filename);
		create_kernel(function_name);
	}

	cl_mem create_raw_buffer(size_t size) {
		cl_int err;
		cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, size, nullptr, &err);
		assert_cl_success(err, "Error creating OpenCL buffer");

		return buffer;
	}

	CL_Buffer create_buffer(size_t size) {
		cl_mem buffer = create_raw_buffer(size);

		return CL_Buffer(buffer, size);
	}

	template <typename T>
	void write_to_buffer(cl_mem buffer, size_t size, T* data) {
		_write_to_buffer(buffer, size, data);
	}

	template <typename T>
	void write_to_buffer(cl_mem buffer, T* data) {
		write_to_buffer(buffer, sizeof(data), data);
	}

	template <typename T>
	void write_to_buffer(CL_Buffer buffer, T* data) {
		write_to_buffer(buffer.buffer, buffer.size, data);
	}

	template <typename T>
	CL_Buffer create_and_write_buffer(size_t size, T* data) {
		CL_Buffer buffer = create_buffer(size);
		write_to_buffer(buffer.buffer, size, data);

		return buffer;
	}

	template <typename T>
	CL_Buffer create_and_write_buffer(T* data) {
		CL_Buffer buffer = create_buffer(sizeof(data));
		write_to_buffer(buffer.buffer, sizeof(data), data);

		return buffer;
	}

	void read_from_buffer(void* data_ptr, CL_Buffer buffer) {
		cl_int err = clEnqueueReadBuffer(queue, buffer.buffer, CL_TRUE, 0, buffer.size, data_ptr,
			0, nullptr, nullptr);
	}

	void read_from_buffer(void* data_ptr, size_t size, cl_mem buffer) {
		cl_int err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, size, data_ptr,
			0, nullptr, nullptr);
	}

	void set_kernel_arg(int index, cl_mem buffer) {
		cl_int err = clSetKernelArg(kernel, (cl_uint)index, sizeof(cl_mem), &buffer);
		assert_cl_success(err, "Error setting OpenCL kernel arg");
	}

	void set_kernel_arg(cl_mem buffer) {
		set_kernel_arg(current_kernel_arg, buffer);
		current_kernel_arg++;
	}

	void set_kernel_arg(int index, CL_Buffer buffer) {
		set_kernel_arg(index, buffer.buffer);
	}

	void set_kernel_arg(CL_Buffer buffer) {
		set_kernel_arg(buffer.buffer);
	}

	void set_global_item_size(size_t global_size) {
		global_item_size = global_size;
	}

	void set_local_item_size(size_t local_size) {
		local_item_size = local_size;
	}

	void set_item_sizes(size_t global_size, size_t local_size) {
		set_global_item_size(global_size);
		set_local_item_size(local_size);
	}

	void run_kernel(size_t global_size, size_t local_size) {
		set_item_sizes(global_size, local_size);
		run_kernel();
	}

	void run_kernel() {
		
		if (global_item_size <= 0) {
			std::cout << "Global item size not initialized!" << "\n";
			exit(1);
		}
		if (local_item_size <= 0) {
			std::cout << "Local item size not initialized!" << "\n";
			exit(1);
		}
		if (global_item_size % local_item_size != 0) {
			std::cout << "Global item size must be divisible by local item size!" << "\n\n";
			exit(1);
		}
		cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, 0, &global_item_size, &local_item_size,
			0, nullptr, nullptr);
		assert_cl_success(err, "Error enqueuing OpenCL kernel");
	}

	void finish_queue() {
		clFinish(queue);
		current_kernel_arg = 0;
	}

private:
	CL_Hardware_Info hardware_info;
	cl_context_properties* cl_context_props;

	std::vector<std::string> required_device_extensions;

	cl_device_id device_id;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	size_t global_item_size;
	size_t local_item_size;

	int current_kernel_arg = 0;

	void init() {
		hardware_info = CL_Hardware_Info(required_device_extensions);

		device_id = _get_device_id();

		context = _create_context();

		queue = _create_command_queue();
	}

	void _assert_program_build_success(const cl_int err) {
		if (err) {
			std::cout << "Error building OpenCL program\n\tCode: " << err << "\n\n";

			cl_int info_err;

			char* log;
			size_t log_length;

			info_err = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_length);

			log = (char*)malloc(log_length);

			info_err = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_length, log, nullptr);

			assert_cl_success(info_err, "Error getting OpenCL program build info");

			std::cout << log << "\n";
			
			exit(1);
		}
	}

	cl_device_id _get_device_id() {
		return hardware_info.device_to_use;
	}

	cl_context _create_context() {
		cl_int err;

		cl_context context = clCreateContext((const cl_context_properties*)cl_context_props, 1, &device_id, nullptr, nullptr, &err);
		assert_cl_success(err, "Error creating OpenCL context");

		return context;
	}

	cl_command_queue _create_command_queue() {
		cl_int err;
		
		cl_command_queue queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
		assert_cl_success(err, "Error creating OpenCL command queue");

		return queue;
	}

	cl_program _create_program(const char* filename) {
		cl_int err;

		std::string s = read_file(filename);
		const char* program_source = s.c_str();
		size_t source_length = 0;

		cl_program program = clCreateProgramWithSource(context, 1, &program_source, &source_length, &err);
		assert_cl_success(err, "Error creating program");

		return program;
	}

	void _build_program() {
		cl_int err = clBuildProgram(program, 1, &device_id, "", nullptr, nullptr);
		_assert_program_build_success(err);
	}

	cl_kernel _create_kernel(const char* function_name) {
		cl_int err;

		cl_kernel kernel = clCreateKernel(program, function_name, &err);
		assert_cl_success(err, "Error creating OpenCL kernel");

		return kernel;
	}

	template <typename T>
	void _write_to_buffer(cl_mem buffer, size_t size, T* data) {
		cl_int err = clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, size, data, 0, nullptr, nullptr);
		assert_cl_success(err, "Error writing to buffer");
	}
};