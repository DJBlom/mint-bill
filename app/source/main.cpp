/********************************************************************************
 * Contents: The main function
 * Author: Dawid Blom
 * Date: October 26, 2024
 *
 * Note:
 *******************************************************************************/
#include <unistd.h>
#include <errors.h>
#include <iostream>
#include <admin.h>


int main(int argc, char** argv)
{
    app::admin admin_app;
    return admin_app.run(argc, argv);
}
