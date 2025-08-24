> # MintBill
> MintBill is an invoice system for two small businesses, T.M Engineering and T.C Engineering.      
> It's domain-specific and highly related to the two firms and their needs. The invoice system            
> covers business registration, new client registration, statement generation, and invoice               
> creation based on work done. The user can create, store, print, and email invoices and 
> statements for specific clients. 
> 
> 
> ## Documentation
> All documents related to the requriements, design, analysis, and risk assessment of the                  
> system should be stored in the docs directory under their respective child directories.                      
> 
> 
> ## Project Tool
> The project tool used to set up the development for the developer is called `mintbill.sh`,                       
> located in the **scripts** directory found in the project base directory.                       
> - For a description of the tool and its capabilities, run `./scripts/mintbill.sh -h`.        
> 
> 
> ## Getting Ready For Development
> Follow the below steps to set up your system for development for this project.          
> **NOTE: Currently, the project supports development on Fedora and Ubuntu.** 
> It's important to note that you will need to call **source** when executing the script
> in the case of using **Ubuntu.** For example, `source ./scripts/mintbill.sh -i develop`                
> - To install all required packages for code development: `./scripts/mintbill.sh -i develop` 
> - To install all required tools for GUI design: `./scripts/mintbill.sh -i design`
> - To install all required tools for analysis: `./scripts/mintbill.sh -i analysis`
> 
> 
> ## Rules For Development
> ### Branching Strategy
> The branching strategy is simple. It consists of three levels known as production                
> (e.g. main branch), integration (e.g. develop branch), and development               
> (e.g. feature/refactor branches).
> 
> Essentially, the code in the main branch is ready for deployment, the code in the                
> develop branch will need to go through integration tests before it can be merged              
> to the main branch for deployment, and the code in the feature/refactor branches       
> are under development.
> 
> A typical flow would be a branch of off-develop and create a feature/branch-name branch.                   
> Develop your feature and merge it into the develop branch for integration testing.                  
> If you want to refactor a piece of code that already exists, branch off the develop              
> branch and create a refactor/branch-name branch. After the refactoring is complete, merge              
> it to the develop branch and run the integration tests again to verify nothing broke.
> 
> ### Branch Naming
> The branches **main** and **develop** are by default already created. Therefore, the only               
> branches you need to create will be feature or refactor branches. Here are the rules for               
> creating such branches:                
> - All branch names shall be lower case seperated by hyphens. (e.g. feature/test-branch)
> - All branches shall have meaningful names that describe the purpose of the branch.                        
>   (e.g. feature/invoice-processing, feature/statement-verification)
>
> # App Password
> The application requires that you create a gmail app password for the email services.    
> This password will not be shared after you have entered into the application.  
> The create a App password with gmail. Ensure that two-factor authentication is turned   
> on in your gmail account. After that, navigate to the following link:   
> - GMail documentation: https://support.google.com/mail/answer/185833?hl=en  
> - Click on **Create and manage your app passwords**.  
> - Enter the password for your application.  
> - Enter the password into this application when registering a business.  
