############################################################################
# Contents: Project Install Functions
# Author: Dawid Blom
# Date: September 15, 2023
# 
# NOTE:
############################################################################
#!/bin/bash


readonly INSTALLATION_OPTION=("develop" "design" "analysis")


function run_install()
{
        local install=$1
        if [[ $install == ${INSTALLATION_OPTION[0]} ]];
        then
                $ECHO "${INFO_COLOR}INFO: Installing required development packages${END_COLOR}"
                $ECHO "${SUCCESS_COLOR}"
                install_develop
                $ECHO "${END_COLOR}"
        elif [[ $install == ${INSTALLATION_OPTION[1]} ]];
        then
                $ECHO "${INFO_COLOR}INFO: Installing 'Design' software${END_COLOR}"
                $ECHO "${SUCCESS_COLOR}"
                install_design
                $ECHO "${END_COLOR}"
        elif [[ $install == ${INSTALLATION_OPTION[2]} ]];
        then
                $ECHO "${INFO_COLOR}INFO: Installing 'Analysis' software${END_COLOR}"
                $ECHO "${SUCCESS_COLOR}"
                install_analysis
                $ECHO "${END_COLOR}"
        else
                $ECHO "${ERROR_COLOR}ERROR: Currently, I do not support that kind of statistics${END_COLOR}"
        fi
}

function install_develop()
{
        dnf_packages=(git cloc lcov gcovr make cmake cppcheck dh-autoreconf automake autoconf gtk4 gtk4-devel
                      libcurl libcurl-devel json-devel flatpak)
        apt_packages=(git cloc lcov gcovr make cmake cppcheck dh-autoreconf automake autoconf libgtk-4-1 
                      libgtk-4-dev gtk-4-examples flatpak gnome-software-plugin-flatpak nlohmann-json3-dev
                      Libcurl-dev libcurl)

        DNF=$(which dnf)
        APT=$(which apt)

        if [[ ! -z $DNF ]];
        then
                sudo dnf update
                sudo dnf -y install ${dnf_packages[@]}
                sudo dnf swap libcurl-minimal libcurl
                install_cpputest
                flatpak remote-add --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
        elif [[ ! -z $APT ]];
        then
                sudo apt -y install ${apt_packages[@]}
                sudo apt update
                install_cpputest
                flatpak remote-add --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
        else
                $ECHO "${ERROR_COLOR}ERROR: This system does not use the dnf based package manage${END_COLOR}"
        fi
}

function install_cpputest()
{
        if [[ -z "$CPPUTEST_HOME" ]];
        then
                local tool_dir=$(pwd)/../
                cd $tool_dir
                git clone https://github.com/cpputest/cpputest.git
                cd cpputest
                autoreconf --install
                ./configure
                make tdd
                echo "export CPPUTEST_HOME=$tool_dir/cpputest/" >> /home/$(whoami)/.bashrc
                source /home/$(whoami)/.bashrc
        else
                $ECHO "${INFO_COLOR}INFO: CppUTest is already installed @ $CPPUTEST_HOME${END_COLOR}"
        fi
}

function install_design()
{
        flatpak install flathub ar.xjuan.Cambalache
}

function install_analysis()
{
        flatpak install flathub org.gaphor.Gaphor
}
