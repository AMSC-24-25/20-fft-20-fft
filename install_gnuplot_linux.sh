#!/bin/bash

if command -v gnuplot > /dev/null 2>&1; then
    echo "Gnuplot is already installed."
else
    echo "Gnuplot is not installed."

    # Prompt user to install gnuplot
    read -p "Do you want to install it? [Y/n] " response
    response=${response,,} # tolower
    if [[ "$response" =~ ^(yes|y| ) ]] || [[ -z "$response" ]]; then
        sudo apt update
        sudo apt install -y gnuplot
        if command -v gnuplot > /dev/null 2>&1; then
            echo "Gnuplot successfully installed."
        else
            echo "Failed to install Gnuplot."
            exit 1
        fi
    else
        echo "Gnuplot installation skipped."
        exit 1
    fi
fi
