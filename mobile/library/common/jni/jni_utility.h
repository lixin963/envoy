#pragma once

#include <string>
#include <vector>

#include "source/common/protobuf/protobuf.h"

#include "library/common/jni/import/jni_import.h"
#include "library/common/jni/jni_helper.h"
#include "library/common/types/c_types.h"
#include "library/common/types/managed_envoy_headers.h"
#include "library/common/types/matcher_data.h"

namespace Envoy {
namespace JNI {

// TODO(Augustyniak): Replace the usages of this global method with Envoy::JNI::Env::get()
JNIEnv* get_env();

void set_class_loader(jobject class_loader);

/**
 * Finds a class with a given name using a class loader provided with the use
 * of `set_class_loader` function. The class loader is supposed to come from
 * application's context and should be associated with project's code - Java classes
 * defined by the project. For finding classes of Java built in-types use
 * `env->FindClass(...)` method instead as it is lighter to use.
 *
 * Read more about why you cannot use `env->FindClass(...)` to look for Java classes
 * defined by the project and a pattern used by the implementation of `find_class` helper
 * method at https://developer.android.com/training/articles/perf-jni#native-libraries.
 *
 * The method works on Android targets only as the `set_class_loader` method is not
 * called by JVM-only targets.
 *
 * @param class_name, the name of the class to find (i.e.
 * "io.envoyproxy.envoymobile.utilities.AndroidNetworkLibrary").
 *
 * @return jclass, the class with a provided `class_name` or NULL if
 *         it couldn't be found.
 */
jclass find_class(const char* class_name);

void jni_delete_global_ref(void* context);

void jni_delete_const_global_ref(const void* context);

int unbox_integer(JniHelper& jni_helper, jobject boxedInteger);

envoy_data array_to_native_data(JniHelper& jni_helper, jbyteArray j_data);

envoy_data array_to_native_data(JniHelper& jni_helper, jbyteArray j_data, size_t data_length);

/**
 * Utility function that copies envoy_data to jbyteArray.
 *
 * @param env, the JNI env pointer.
 * @param envoy_data, the source to copy from.
 *
 * @return jbyteArray, copied data.
 */
LocalRefUniquePtr<jbyteArray> native_data_to_array(JniHelper& jni_helper, envoy_data data);

LocalRefUniquePtr<jlongArray> native_stream_intel_to_array(JniHelper& jni_helper,
                                                           envoy_stream_intel stream_intel);

LocalRefUniquePtr<jlongArray>
native_final_stream_intel_to_array(JniHelper& jni_helper,
                                   envoy_final_stream_intel final_stream_intel);

/**
 * Utility function that copies envoy_map to a java HashMap jobject.
 *
 * @param env, the JNI env pointer.
 * @param envoy_map, the source to copy from.
 *
 * @return jobject, copied data.
 */
LocalRefUniquePtr<jobject> native_map_to_map(JniHelper& jni_helper, envoy_map map);

LocalRefUniquePtr<jstring> native_data_to_string(JniHelper& jni_helper, envoy_data data);

envoy_data buffer_to_native_data(JniHelper& jni_helper, jobject j_data);

envoy_data buffer_to_native_data(JniHelper& jni_helper, jobject j_data, size_t data_length);

envoy_data* buffer_to_native_data_ptr(JniHelper& jni_helper, jobject j_data);

envoy_headers to_native_headers(JniHelper& jni_helper, jobjectArray headers);

envoy_headers* to_native_headers_ptr(JniHelper& jni_helper, jobjectArray headers);

envoy_stats_tags to_native_tags(JniHelper& jni_helper, jobjectArray tags);

envoy_map to_native_map(JniHelper& jni_helper, jobjectArray entries);

/**
 * Utilities to translate C++ std library constructs to their Java counterpart.
 * The underlying data is always copied to disentangle C++ and Java objects lifetime.
 */
LocalRefUniquePtr<jobjectArray> ToJavaArrayOfByteArray(JniHelper& jni_helper,
                                                       const std::vector<std::string>& v);

LocalRefUniquePtr<jbyteArray> ToJavaByteArray(JniHelper& jni_helper, const uint8_t* bytes,
                                              size_t len);

LocalRefUniquePtr<jbyteArray> ToJavaByteArray(JniHelper& jni_helper, const std::string& str);

LocalRefUniquePtr<jobjectArray>
ToJavaArrayOfObjectArray(JniHelper& jni_helper, const Envoy::Types::ManagedEnvoyHeaders& map);

void JavaArrayOfByteArrayToStringVector(JniHelper& jni_helper, jobjectArray array,
                                        std::vector<std::string>* out);

void JavaArrayOfByteToBytesVector(JniHelper& jni_helper, jbyteArray array,
                                  std::vector<uint8_t>* out);

void JavaArrayOfByteToString(JniHelper& jni_helper, jbyteArray jbytes, std::string* out);

/** Parses the proto from Java's byte array and stores the output into `dest` proto. */
void javaByteArrayToProto(JniHelper& jni_helper, jbyteArray source,
                          Envoy::Protobuf::MessageLite* dest);

} // namespace JNI
} // namespace Envoy
