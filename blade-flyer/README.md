Blade Flyer
===========

Project by **Alexandre Gomes** & **Brice Thomas**, 2014

Compile
-------

You'll need GLPK and probably to extend your stack size with the following command :
```ulimit -s 65000``` (you can put whatever you want but 65000 seems enough)

Then to compile :
```g++ -std=c++0x algo_enum.cpp -o algo_enum -lglpk -lm -O2```

Launch
------

```./algo_enum path_to_data_file```
