#include <business_feature.h>



void feature::business::load(data::business& data)
{
        data.set_name("T.M Engineering");
        data.set_address("Geelsterd 8");
        data.set_area_code("6536");
        data.set_town("george");
        data.set_cellphone("0832315944");
        data.set_email("odn@gmail.com");
        data.set_bank("Standard Bank Oudtshoorn");
        data.set_branch_code("050514");
        data.set_account_number("371613191");
        data.set_client_message("Thank you for your support.");
}

bool feature::business::save(data::business& data)
{
        bool saved{false};
        if (data.is_valid() == true)
        {
                saved = true;
        }

        return saved;
}
