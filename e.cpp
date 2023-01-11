#include <iostream>

struct user
{
    const char *name;
    int id;

    user(const char *name, int id)
    {
        this->name = name;
        this->id = id;
    }

    void hello()
    {
        std::cout << "Hello world\n";
    }
    protected:
    void hi()
    {
        printf("hello world\n");
    }
};

void t(user **u)
{
    **u = user("gurgui", 1);
};

int main()
{
}