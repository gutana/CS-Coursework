#include <iostream>
#include <vector>

bool elementsAreDistinct(const std::vector<int> &v)
{
    for (int i = 1; i < v.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (v[i] == v[j])
                return false;
        }
    }

    return true;
}

int main()
{
    std::vector<int> array = {1, 1};

    std::cout << "Array elements are distinct: " << (elementsAreDistinct(array) ? "true\n" : "false\n");
}