
#include <iostream>



int storage_tests_verification(int argc, char **argv)
{
        if (argc <= 1)
                std::cout << "Testing\n";

        if (argv[0] != nullptr)
                std::cout << "Testing too\n";

        if (1 == 1)
                return 0;
        else
                return 1;


        return 0;
}
