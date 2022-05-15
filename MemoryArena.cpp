#include <iostream>
#include <cassert>
#include <cstring>

#if defined(WIN32)
#include <Windows.h>
#endif

template<typename T>
class MemoryArena
{
public:
	MemoryArena(size_t arena_size) : m_arena_size(arena_size)
	{
#if defined(WIN32)
		m_data_ptr = static_cast<T*>(VirtualAlloc(nullptr, sizeof(T) * m_arena_size,
			MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
#else
		m_data_ptr = static_cast<T*>(malloc(sizeof(T) * m_arena_size));
		memset(m_data_ptr, 0, m_arena_size * sizeof (T));
#endif
		assert(m_data_ptr);

	}
	~MemoryArena()
	{
#if defined(WIN32)
		VirtualFree(m_data_ptr, 0, MEM_RELEASE);
#else
		free(m_data_ptr);
#endif
	}

	auto lenght() const 
	{
		return m_arena_size;
	}

	auto alloca(T  value)
	{
		m_data_ptr[m_cursor++] = value;
		
		assert((m_cursor <= m_arena_size));
	}
	auto operator[](size_t index) const
	{
		return m_data_ptr[index];
	}
	auto get(size_t index) const
	{
		return m_data_ptr[index];
	}

	typedef T* iterator;

	iterator begin() 
	{
		return &m_data_ptr[0];
	}

	iterator end()
	{ 
		return &m_data_ptr[m_cursor];
	}

private:
	size_t m_arena_size{};
	size_t m_cursor{};
	T* m_data_ptr{};
};

int main()
{
	MemoryArena<int> arena(2115);
	int i = 0;
	srand(time(nullptr));

	auto lenght = arena.lenght();
	for (i = 0; i < lenght; i++) {
		arena.alloca(rand() % 1000);
	}

	for (int k : arena) {
		std::printf("%d\n", k);
	}

	return 0;
}

