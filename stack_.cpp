#include <iostream>
#include <exception>
template <typename T>
class stack
{
private:
	struct Node //Элемент стека
	{
		T data;
		Node* next;//Указатель на следующий элемент стека
		Node()
		{
			data = NULL;
			next = NULL;
		}
		Node(T data)
		{
			this->data = data;
		}
		Node(T data, Node* next)
		{
			this->data = data;
			this->next = next;
		}
	};
	Node* top;//Верхний элемент стека
	int size;//Размер стека
public:
	stack()
	{
		top = NULL;
		size = 0;
	}
	~stack() {}
	template <typename ... Args>
	void push_emplace(Args&&... value)
	{
		if (top != NULL)
		{
			Node* temp = new Node(T(std::forward<Args>(value)...), top);
			top = temp;
		}
		else
			top = new Node(T(std::forward<Args>(value)...));
		size++;
	}
	void push(T&& value)
	{
		if (top != NULL)
		{
			Node* temp = new Node(std::move(value), top);
			top = temp;
		}
		else
			top = new Node(std::move(value));
		size++;
	}
	void push(const T& value)
	{
		if (top != NULL)
		{
			Node* temp = new Node(value, top);
			top = temp;
		}
		else
			top = new Node(value);
		size++;
	}
	void pop()
	{
		try
		{
			if (top == NULL)
				throw std::out_of_range("Error: Out of range");
			Node* temp = top;
			top = top->next;
			delete temp;
			size--;
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	const T& head() const
	{
		try
		{
			if (top == NULL)
				throw std::out_of_range("Error: Out of range");
			return top->data;
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	void print()
	{
		Node* temp = top;
		while (size > 0)
		{
			std::cout << temp->data << std::endl;
			if (size > 1)
				temp = temp->next;
			size--;
		}
		std::cout << std::endl;
	}
};
int main()
{
	stack <int> Stack;
	Stack.push(1);
	Stack.push(2);
	Stack.push(3);
	Stack.print();
	std::cout << "head:" << std::endl;
	std::cout << Stack.head() << std::endl;
	Stack.pop();
	std::cout << std::endl << "delete_element." << std::endl;
	std::cout << "head:" << std::endl;
	std::cout << Stack.head() << std::endl;
	Stack.push_emplace<int>(4);
	std::cout << std::endl << "push_element." << std::endl;
	std::cout << "head:" << std::endl;
	std::cout << Stack.head() << std::endl;
	return 0;
}