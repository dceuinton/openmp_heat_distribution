#!/bin/bash

git add .
git status
echo -e "Please enter a commit message:"
read message 
git commit -m "$message"