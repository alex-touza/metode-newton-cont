# Mètode de Newton per a zeros de funcions amb continuació

Aquest és un programa basat en la llibreria de diferenciació i àlgebra lineal
[**Adept**](https://www.met.reading.ac.uk/clouds/adept/) que calcula les solucions d'un sistema no lineal d'equacions
que depenen d'un paràmetre.

Es tracta d'un exercici opcional de la tercera prova d'avaluació continuada de l'assignatura Càlcul Diferencial.

La característica destacable d'aquest programa és el fet que no està estretament lligat a la funció que es vol
estudiar, sinó que es rep com a paràmetre de l'algorisme i no cal calcular a mà la seva matriu jacobiana. D'aquesta
manera, és ben senzill modificar el vostre programa per a trobar solucions d'un altre sistema.

## Requeriments

- CMake
- Compilador de C++ raonable
- Adept 2.1 o superior (per la diferenciació automàtica)
- BLAS (pel producte de matrius)
- LAPACK (per inversió de matrius i resolució de sistemes lineals)

Les instruccions per instal·lar aquestes tres últimes llibreries es poden
trobar [aquí](https://www.met.reading.ac.uk/clouds/adept/adept_documentation.pdf#page=4&zoom=100,96,820). Depenent del
vostre sistema, pot ser un procés força complex.

## Execució

Després de clonar el repositori, executeu aquestes comandes per compilar el programa:

```shell
mkdir build
cd build
cmake ..
make
```

O bé, si disposeu d'un entorn de desenvolupament integrat raonable, podeu obrir-hi el projecte i compilar-lo ràpidament.

## Mètode

Sigui $f : G \subset \mathbb{R}^n \times \mathbb{R} \rightarrow \mathbb{R}^n$ una funció de classe $\mathcal{C}^1 (G)$.
El **mètode de Newton amb continuació** consisteix en trobar els punts $(x,p) \in \mathbb{R}^n \times \mathbb{R}$ tals
que $f(x, p) = 0$ a partir d'una solució ja coneguda $(x_0,p_0)$.

El mètode es basa, essencialment, en dos passos que es repeteixen tantes vegades com el nombre de solucions que hom
vulgui trobar:

- **Continuació.** Sigui $p_i = p_{i-1} + h$ per a algun $h \in \mathbb{R}$ petit. Calculem una primera
  aproximació $z^{(0)} \in \mathbb{R}^n$ de $f(z^{(0)}, p_i) = 0$ donada per l'aproximació lineal
  $z^{(0)} := x(p_0) + h \dfrac{\mathrm{d}x}{\mathrm{d}p}(p_0)$ obtinguda a partir de la derivació de la funció
  implícita.
- **Aproximació (mètode de Newton).** Anem afinant les aproximacions resolent, al pas $k$-èsim, el
  sistema $Df\left(z^{(k-1)}, p_i\right)\left(z - z^{(k-1)}\right) = - f\left(z^{(k-1)}, p_i\right)$,
  on $Df\left(z^{(k-1)}, p_i\right)$ és el diferencial de $f$ respecte $z^{(k-1)}$, fins que el residu
  compleix $\left\lVert f(z^{(k-1)}, p_i) \right\rVert < \varepsilon$.

## Implementació

L'autor ha implementat el mètode a la classe [`newton_method_continuation`](./src/newton_method_continuation.hpp), que
rep com a _template parameter_ la dimensió $n$ del sistema especificat abans. El constructor rep, com a `std::function`,
la funció amb què hom vol treballar, que pot ser còmodament especificada en una _lambda_ (vegeu-ne un exemple a [
`main.cpp`](./src/main.cpp)).

Les funcions `newton_method_continuation::continuation` i
`newton_method_continuation::step` executen els dos passos explicats a la secció anterior, respectivament. La funció
`newton_method_continuation::calculate` duu a terme tot el procés per trobar una nova solució, podent especificar com a
paràmetre una solució coneguda si no n'hi havia cap.

A [`main.cpp`](./src/main.cpp) s'aplica el mètode a la funció proposada a la prova d'avaluació continuada. En executar
el programa, se us demanarà el nombre de solucions que voleu trobar i el salt $h$ que voleu utilitzar.

## Autoria

Àlex Xavier Touza Luque
