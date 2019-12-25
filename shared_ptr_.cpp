#include <iostream>
#include <atomic>
class counter
{
private:
	std::atomic_uint count;
public:
	counter()
	{
		count = 0;
	}
	explicit counter(unsigned int count)
	{
		this->count = count;
	}
	void increase()
	{
		count++;
	}
	void decrease()
	{
		count--;
	}
	bool operator==(const int& c) const
	{
		return (count == c);
	}
	size_t size() 
	{ 
		return static_cast<size_t>(count);
	}
};
template <typename T>
class SharedPtr
{
private:
	counter* count_;
	T* data;
public:
	SharedPtr()
	{
		count_ = nullptr;
		data = nullptr;
	}
	explicit SharedPtr(T* ptr)
	{
		count_ = new counter(1);
		data = ptr;
	}
	SharedPtr(const SharedPtr& r)
	{
		count_ = r.count_;
		data = r.data;
		if (count_ != nullptr)
			count_->increase();
	}
	SharedPtr(SharedPtr&& r)
	{
		count_ = r.count_;
		data = r.data;
		if (count_ != nullptr)
			count_->increase();
		r.count_ = nullptr;
		r.data = nullptr;
	}
	~SharedPtr()
	{
		if (count_ != nullptr)
		{
			count_->decrease();
		}
		if (count_ == 0)
		{
			delete count_;
			delete data;
		}
	}
	auto operator=(const SharedPtr& r)->SharedPtr&
	{
		if (data !=nullptr)
		{
			count_->decrease();
			if (count_ == 0)
			{
				delete count_;
				delete data;
			}
		}
		count_ = r.count_;
		data = r.data;
		return(*this);
	}
	auto operator=(SharedPtr&& r)->SharedPtr&
	{
		if (data != nullptr)
		{
			count_->decrease();
			if (count_ == 0)
			{
				delete count_;
				delete data;
			}
		}
		count_ = r.count_;
		data = r.data;
		if (r.data != nullptr)
		{
			delete r.count_;
			delete r.data;
		}
		return(*this);
	}
	operator bool() const
	{
		if (count_ == 0)
		{
			return false;
		}
		if (data == 0)
		{
			return false;
		}
		return true;
	}
	auto operator*() const->T&
	{
		return *data;
	}
	auto operator->() const->T*
	{
		return data;
	}
	auto get()->T*
	{
		return data;
	}
	void reset()
	{
		if (count_!=nullptr)
		{
			count_->decrease();
			if (count_ == 0)
			{
				delete count_;
				delete data;
			}
		}
		count_ = nullptr;
		data = nullptr;
	}
	void reset(T* ptr)
	{
		if (count_ != nullptr)
		{
			count_->decrease();
			if (count_ == 0)
			{
				delete count_;
				delete data;
			}
		}
		count_ = nullptr;
		data = nullptr;
		count_ = new counter(1);
		data = ptr;
	}
	void swap(SharedPtr& r)
	{
		if (data == r.data)
			return;
		auto temp = r.data;
		r.data = data;
		data = temp;
		auto temp2 = r.count_;
		r.count_ = count_;
		count_ = temp2;
	}
	auto use_count() const->size_t
	{
		if (!*this)
			return 0;
		return count_->size();
	}
};
int main()
{
int* p = new int(5);
SharedPtr<int> ptr1(p);
SharedPtr<int> ptr2(ptr1);
SharedPtr<int> ptr3(ptr2);
SharedPtr<int> ptr4(ptr3);
std::cout << ptr1.use_count() << std::endl;
ptr3.reset();
ptr4.reset();
std::cout << ptr3.use_count() << std::endl;
SharedPtr<int> ptr5(ptr4);
std::cout << ptr1.use_count() << std::endl;
std::cout << ptr5.use_count() << std::endl;
std::cout << ptr2.use_count() << std::endl;
system("pause");
return 0;
}