
#include <iostream>



int utility_tests_test(int argc, char **argv)
{
        if (argc <= 1)
                std::cout << "Testing\n";

        if (argv[0] != nullptr)
                std::cout << "Testing too\n";

        if (3 >= 2)
                return 0;
        else
                return 1;


        return 0;
}
