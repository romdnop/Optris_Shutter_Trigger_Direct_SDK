# About

A python script to convert *.csv images into *.png


# Preparation

1) Install virtual environment

    python -m venv venv

2) Activate it 

In cmd

    venv\Scripts\activate 

OR

in PowerShell

    venv\Scripts\Activate.ps1

3) Install dependencies

    pip install numpy matplotlib opencv-python

*OR*

    pip install -r requirements.txt


To verify installation of the dependencies

    pip list


# Usage

    python csv_to_png.py thermal_1.csv

OR


    python csv_to_png.py thermal_1.csv -o output.png --colormap jet


# Test

    python csv_to_png.py test/thermal.csv -o test/thermal.png --colormap jet

OR

    python csv_to_png.py -d test/ -c plasma