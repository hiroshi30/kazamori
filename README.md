# kazamori
I want to write a theory manual about my project. It will be helpful for newbies in Neural Networks. :)

???:
1. Контекстные нейроны

RBF networks have several features
1. Only one hidden layer
2. Only hidden layers have non-linear activation function
3. The synaptic connection weights of the input and hidden layers are equal to one
4. выходной слой представляет из себя линейную комбинацию выходов скрытого слоя, то есть ...

The synaptic connection weights - just normal weights

MLP - multilayer perceptron

PSO algorithm

RBF
https://libeldoc.bsuir.by/bitstream/123456789/52761/1/Glinskii_Iskusstvennie.pdf
https://yandex.ru/q/machine-learning/12329890561/
https://ru.wikipedia.org/wiki/Базисная_функция
https://learn.microsoft.com/ru-ru/archive/msdn-magazine/2013/december/test-run-radial-basis-function-network-training
https://learn.microsoft.com/en-us/archive/msdn-magazine/2013/october/test-run-radial-basis-function-networks-for-programmers


## Glinskii_Iskusstvennie
### CMake SDL2 building
```bat
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DSDL_TEST=OFF
cmake --build build
```
