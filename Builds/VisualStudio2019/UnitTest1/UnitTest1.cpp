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
			const int VECSIZE = 50;
			const size_t DATASIZE = 120;
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
			const int VECSIZE = 50;
			const size_t DATASIZE = 100;
			auto vec = RingBufferVector<int>(VECSIZE);

			for (int i = 0; i < DATASIZE; ++i)
			{
				vec.writeValue(i);
			}

			auto vector = std::vector<int>(DATASIZE);
			vec.readPreviousValues(vector);

			for (int i = 0; i < DATASIZE; ++i)
			{
				Assert::IsTrue(vec[i % VECSIZE] == vector[i]);
			}
		}

		TEST_METHOD(DUMMY)
		{
			Assert::IsTrue(true);
		}
	};
}
