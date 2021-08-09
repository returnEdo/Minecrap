#pragma once


namespace Memory
{


template <typename T>
class Unique
{
	private:

	T* mPointer { nullptr };

	public:
	
	~Unique(void)
	{
		if (mPointer)
		{
			delete mPointer;
		}
	};

	template <typename U, class ... Types>
	void allocate(Types ... tArgs)
	{
		mPointer = new U(tArgs ...);
	};

	template <typename U>
	U* get(void)
	{
		return (U*)(mPointer);
	};
	
	T* get(void)
	{
		return mPointer;
	};

};


}
