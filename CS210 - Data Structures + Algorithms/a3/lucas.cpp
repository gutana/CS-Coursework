#include <iostream>
#include <vector>

int lucas(int num)
{
    if (num == 1)
        return 1;
    if (num == 0)
        return 2;

    return (lucas(num - 1) + lucas(num - 2));
}

int lucasIt(int num)
{
    std::vector<int> numArray = {2, 1};
    // numArray.reserve(num);

    for (int i = 2; i < num + 1; i++)
    {
        numArray[i] = numArray[i - 1] + numArray[i - 2];
    }
    return numArray[num];
}

int main()
{
    while (true)
    {
        int num;
        std::cout << "Enter number: ";
        std::cin >> num;

        std::cout << "Lucas number: " << lucasIt(num) << '\n';
    }
}