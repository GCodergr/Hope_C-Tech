#pragma once

#include <iostream>

// Comment out to disable logging
#define LOG_ENABLED 1

struct AllocationMetrics
{
	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;

	uint32_t CurrentUsage()
	{
		return TotalAllocated - TotalFreed;
	}
};

static AllocationMetrics allocation_metrics;

inline void* operator new(size_t size)
{
#if LOG_ENABLED
	std::cout << "Allocating " << size << " bytes\n";
#endif	

	allocation_metrics.TotalAllocated += size;

	return malloc(size);
}

inline void operator delete(void* memory, size_t size)
{
#if LOG_ENABLED
	std::cout << "Freeing " << size << " bytes\n";
#endif

	allocation_metrics.TotalFreed += size;

	free(memory);
}

static void PrintMemoryUsage()
{
#if LOG_ENABLED
	std::cout << "Memory Usage: " << allocation_metrics.CurrentUsage() << " bytes\n";
#endif
}