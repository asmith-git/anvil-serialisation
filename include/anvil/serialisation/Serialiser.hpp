// MIT License
// 
// Copyright(c) 2021 Adam Smith
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef ANVIL_SERIALISATION_SERIALISER_HPP
#define ANVIL_SERIALISATION_SERIALISER_HPP

#include <cstdint>

namespace anvil {
	class Serialiser {
	public:
		enum Type : uint8_t {
			TYPE_UNSIGNED_8,
			TYPE_UNSIGNED_16,
			TYPE_UNSIGNED_32,
			TYPE_UNSIGNED_64,
			TYPE_SIGNED_8,
			TYPE_SIGNED_16,
			TYPE_SIGNED_32,
			TYPE_SIGNED_64,
			TYPE_FLOAT_32,
			TYPE_FLOAT_64,
			TYPE_STRING,
			TYPE_ARRAY,
			TYPE_OBJECT
		};

		Serialiser() {}
		virtual ~Serialiser() {}

		virtual void SetNextValueU8(const uint8_t value) = 0;
		virtual void SetNextValueU16(const uint16_t value) = 0;
		virtual void SetNextValueU32(const uint32_t value) = 0;
		virtual void SetNextValueU64(const uint64_t value) = 0;
		virtual void SetNextValueS8(const int8_t value) = 0;
		virtual void SetNextValueS16(const int16_t value) = 0;
		virtual void SetNextValueS32(const int32_t value) = 0;
		virtual void SetNextValueS64(const int64_t value) = 0;
		virtual void SetNextValueF32(const float value) = 0;
		virtual void SetNextValueF64(const double value) = 0;
		virtual void SetNextValueString(const char* value) = 0;

		virtual void StartArray() = 0;
		virtual void EndArray() = 0;

		virtual void StartObject() = 0;
		virtual void EndObject() = 0;

		virtual void SetNextMemberName(const char* name) = 0;

		// Object name helpers
	
		inline void SetNextValueU8(const char* name, const uint8_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueU8(value);
		}
	
		inline void SetNextValueU16(const char* name, const uint16_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueU16(value);
		}
	
		inline void SetNextValueU32(const char* name, const uint32_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueU32(value);
		}
	
		inline void SetNextValueU64(const char* name, const uint64_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueU64(value);
		}
	
		inline void SetNextValueS8(const char* name, const int8_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueS8(value);
		}
	
		inline void SetNextValueS16(const char* name, const int16_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueS16(value);
		}
	
		inline void SetNextValueS32(const char* name, const int32_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueS32(value);
		}
	
		inline void SetNextValueS64(const char* name, const int64_t value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueS64(value);
		}
	
		inline void SetNextValueF32(const char* name, const float value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueF32(value);
		}
	
		inline void SetNextValueF64(const char* name, const double value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueF64(value);
		}
	
		inline void SetNextValueString(const char* name, const char* value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValueString(value);
		}
	
		inline void StartArray(const char* name) {
			Serialiser::SetNextMemberName(name);
			Serialiser::StartArray();
		}
	
		inline void StartObject(const char* name) {
			Serialiser::SetNextMemberName(name);
			Serialiser::StartArray();
		}

		// Array optimisations

		virtual void SetNextValueU8(const uint8_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueU8(value[i]);
		}

		virtual void SetNextValueU16(const uint16_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueU16(value[i]);
		}

		virtual void SetNextValueU32(const uint32_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueU32(value[i]);
		}

		virtual void SetNextValueU64(const uint64_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueU64(value[i]);
		}

		virtual void SetNextValueS8(const int8_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueS8(value[i]);
		}

		virtual void SetNextValueS16(const int16_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueS16(value[i]);
		}

		virtual void SetNextValueS32(const int32_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueS32(value[i]);
		}

		virtual void SetNextValueS64(const int64_t* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueS64(value[i]);
		}

		virtual void SetNextValueF32(const float* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueF32(value[i]);
		}

		virtual void SetNextValueF64(const double* value, const size_t count) {
			for (size_t i = 0; i < count; ++i) Serialiser::SetNextValueF64(value[i]);
		}

		// Template helpers

		template<class T>
		inline void SetNextValue(T value);

		template<class T>
		inline void SetNextValue(const T* value, const uint32_t count);

		template<class T>
		inline void SetNextValue(const char* name, T value) {
			Serialiser::SetNextMemberName(name);
			Serialiser::SetNextValue<T>(value);
		}

		template<class T>
		inline void SetNextValue(const char* name, const T* value, const uint32_t count) {
			// A named array is a special case so we will also call EndArray
			Serialiser::StartArray(name);
			Serialiser::SetNextValue<T>(value, count);
			Serialiser::EndArray();
		}

		template<>
		inline void SetNextValue<uint8_t>(uint8_t value) {
			Serialiser::SetNextValueU8(value);
		}

		template<>
		inline void SetNextValue<uint8_t>(const uint8_t* value, const uint32_t count) {
			Serialiser::SetNextValueU8(value, count);
		}

		template<>
		inline void SetNextValue<uint16_t>(uint16_t value) {
			Serialiser::SetNextValueU16(value);
		}

		template<>
		inline void SetNextValue<uint16_t>(const uint16_t* value, const uint32_t count) {
			Serialiser::SetNextValueU16(value, count);
		}

		template<>
		inline void SetNextValue<uint32_t>(uint32_t value) {
			Serialiser::SetNextValueU32(value);
		}

		template<>
		inline void SetNextValue<uint32_t>(const uint32_t* value, const uint32_t count) {
			Serialiser::SetNextValueU32(value, count);
		}

		template<>
		inline void SetNextValue<uint64_t>(uint64_t value) {
			Serialiser::SetNextValueU64(value);
		}

		template<>
		inline void SetNextValue<uint64_t>(const uint64_t* value, const uint32_t count) {
			Serialiser::SetNextValueU64(value, count);
		}

		template<>
		inline void SetNextValue<int8_t>(int8_t value) {
			Serialiser::SetNextValueS8(value);
		}

		template<>
		inline void SetNextValue<int8_t>(const int8_t* value, const uint32_t count) {
			Serialiser::SetNextValueS8(value, count);
		}

		template<>
		inline void SetNextValue<int16_t>(int16_t value) {
			Serialiser::SetNextValueS16(value);
		}

		template<>
		inline void SetNextValue<int16_t>(const int16_t* value, const uint32_t count) {
			Serialiser::SetNextValueS16(value, count);
		}

		template<>
		inline void SetNextValue<int32_t>(int32_t value) {
			Serialiser::SetNextValueS32(value);
		}

		template<>
		inline void SetNextValue<int32_t>(const int32_t* value, const uint32_t count) {
			Serialiser::SetNextValueS32(value, count);
		}

		template<>
		inline void SetNextValue<int64_t>(int64_t value) {
			Serialiser::SetNextValueS64(value);
		}

		template<>
		inline void SetNextValue<int64_t>(const int64_t* value, const uint32_t count) {
			Serialiser::SetNextValueS64(value, count);
		}

		template<>
		inline void SetNextValue<float>(float value) {
			Serialiser::SetNextValueF32(value);
		}

		template<>
		inline void SetNextValue<float>(const float* value, const uint32_t count) {
			Serialiser::SetNextValueF32(value, count);
		}

		template<>
		inline void SetNextValue<double>(double value) {
			Serialiser::SetNextValueF64(value);
		}

		template<>
		inline void SetNextValue<double>(const double* value, const uint32_t count) {
			Serialiser::SetNextValueF64(value, count);
		}
	};
}

#endif