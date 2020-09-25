#!/bin/bash 

run_git_cmd(){
    echo "$*"
    $*
}


result="$(run_git_cmd git status)"                  
echo $result
exit 0

#
#
#run_git_cmd() {
#    echo "$*"
#    #$1
#    #echo. 
#}
#
#run_git_cmd("aaa")

#echo git status
#git status
#
#echo git status
#git add .
#git commit -m "update daily"
#git push git@github.com:93873637/Android.git
#git status
