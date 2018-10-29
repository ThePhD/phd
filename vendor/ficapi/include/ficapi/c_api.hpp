// ficapi��
// The Shrug License

// Copyright � 2018 ThePhD. All rights can sometimes maybe not unreserved, yo.

#pragma once

#ifndef FICAPI_C_API_HPP
#define FICAPI_C_API_HPP

#if defined _MSC_VER
#define FICAPI_VC
#elif defined __GNUC__
#define FICAPI_GCC
#elif defined __clang__
#define FICAPI_CLANG
#endif

#if defined FICAPI_VC
#if defined FICAPI_DLL
#if defined FICAPI_BUILD
#define FICAPI_API __declspec(dllexport)
#else
#define FICAPI_API __declspec(dllexport)
#endif // FICAPI_BUILD - Building the Library vs. Using the Library
#else
#define FICAPI_API
#endif // Building a DLL vs. Static Library
#else  // g++ / clang++
#define FICAPI_API __attribute__((visibility("default")))
#endif // FICAPI_BUILD

#if __cplusplus
extern "C" {
#endif // C++

struct ficapi_opaque_;
typedef struct ficapi_opaque_ ficapi_opaque;
typedef ficapi_opaque* ficapi_opaque_handle;
typedef ficapi_opaque const* ficapi_opaque_handle_const;

enum ficapi_type {
	ficapi_type_int,
	ficapi_type_opaque
};

FICAPI_API void ficapi_create(void** p_handle, ficapi_type ft);
FICAPI_API void ficapi_re_create(void** p_handle, ficapi_type ft);
FICAPI_API void ficapi_delete(void* handle, ficapi_type ft);
FICAPI_API void ficapi_int_create(int** p_handle);
FICAPI_API void ficapi_int_re_create(int** p_handle);
FICAPI_API void ficapi_int_delete(int* handle);
FICAPI_API void ficapi_handle_create(ficapi_opaque_handle* p_handle);
FICAPI_API void ficapi_handle_re_create(ficapi_opaque_handle* p_handle);
FICAPI_API void ficapi_handle_delete(ficapi_opaque_handle handle);
FICAPI_API void ficapi_handle_no_alloc_create(ficapi_opaque_handle* p_handle);
FICAPI_API void ficapi_handle_no_alloc_re_create(ficapi_opaque_handle* p_handle);
FICAPI_API void ficapi_handle_no_alloc_delete(ficapi_opaque_handle handle);

FICAPI_API int ficapi_handle_get_data(ficapi_opaque_handle_const handle);

FICAPI_API void ficapi_set_data(int);
FICAPI_API int ficapi_get_data();

FICAPI_API void ficapi_set_dynamic_data(int);
FICAPI_API int ficapi_get_dynamic_data();

#if __cplusplus
}
#endif // C++

#endif // FICAPI_C_API_HPP
