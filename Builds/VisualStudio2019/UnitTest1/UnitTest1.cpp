#include "pch.h"
#include <vector>
#include "CppUnitTest.h"
#include "RingBuffer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test 
{
	TEST_CLASS(RingBuffer)
	{
	public:
		
		TEST_METHOD(VectorIO)
		{
			const size_t VECSIZE = 5;
			const size_t DATASIZE = 10;
			auto vec = RingBufferVector<int>(VECSIZE);

			for (int i = 0; i < DATASIZE; ++i)
			{
				vec.writeValue(i);
			}

			for (int i = 0; i < VECSIZE; ++i)
			{
				const int value = vec.readValue();
				Assert::IsTrue(value == DATASIZE - VECSIZE + i);
			}
		}

		TEST_METHOD(VectorPrevious)
		{
			const int VECSIZE = 5;
			const size_t DATASIZE = 12;
			auto vec = RingBufferVector<int>(VECSIZE);

			for (int i = 0; i < DATASIZE; ++i)
			{
				vec.writeValue(i);
			}

			auto vector = std::vector<int>(VECSIZE);
			vec.readPreviousValues(vector);

			for (int i = 0; i < VECSIZE; ++i)
			{
				Assert::IsTrue(vec[i]  == vector[i]);
			}
		}

	};
}
