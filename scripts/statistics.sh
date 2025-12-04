################################################################################
# File: statistics.sh
# Description:
#   Provides project-level statistical tools such as line-of-code (LOC)
#   reporting using 'cloc'. Intended for quick codebase metrics and size
#   analysis functions.
#
# Features:
#   - LOC measurement with cloc
#   - Easy extension point for additional metrics
#
# Usage:
#   ./statistics.sh <stats-type>
#
#   Supported stat types:
#       lc – Count total project lines of code
#
################################################################################
#!/bin/bash


readonly STATS_TYPES=("lc")


function run_statistics()
{
        local statstype=$1
        if [[ $statstype == ${STATS_TYPES[0]} ]];
        then
                $ECHO "${INFO_COLOR}INFO: Running 'Cloc' to see the projects line count${END_COLOR}"
                line_count
        else
                $ECHO "${ERROR_COLOR}ERROR: Currently, I do not support that kind of statistics${END_COLOR}"
        fi
}

function line_count()
{
        $ECHO "${INFO_COLOR}INFO: Total project line count"
        cloc .
        $ECHO "${END_COLOR}"
}
