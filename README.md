# Détermination de la précision nécessaire

## Troncature

Pour $x\in\mathbb{R}$ et $p\in\mathbb{Z}$, on définit une opération de troncature, notée $x_l$, qui consiste à ne garder de $x$ que les bits  jusqu'a $2^l$ inclus : 
$$
x_l = \lfloor x/2^{l} \rfloor \times 2^{l}
$$
Remarque : si $l>0$, cela revient à remplacer par des zéros les $l$ bits avant la virgule.

## Troncature majorée

Pour $x\in\mathbb{R}$ et $p\in\mathbb{Z}$, on définit une opération de troncature majorée, notée $\overline{x_l}$  : 
$$
\overline{x_l} = \lfloor x/2^{l}+1 \rfloor \times 2^{l}
$$

Propriété : $x_l<\overline{x_l}$

## Intervalle de troncature

A tout nombre tronqué $x_l$ on peut associer un intervalle  :  
$$
\mathrm{Interval}(x_l)=[x_l,\overline{x_l}[
$$
contenant tous les réels de même troncature que $x$ à $p$ bits, de sorte que : 
$$
\forall u\in[x_l,\overline{x_l}[\Leftrightarrow  u_l=x_l
$$

## Plus grande précision commune

Pour un intervalle $[l,h[$ on peut calculer une *plus grande précision commune* à toutes les valeurs de l'intervalle $p\in\mathbb{Z}$ telle que :
$$
\mathrm{pgpc}([l,h[) = p\equiv\forall u,v \in [l,h[, (u_l=v_l) \& (u_{p+1}\neq v_{p+1})
$$

## Découpage d'un intervalle

Tronquer les valeurs d'un intervalle 
$$
[l,h]_l = \{x_l \mid x\in [l,h]\}
$$

## Précision nécessaire

Soit une fonction $F(x)$ dont l'argument $x$, exprimé avec une précision $p$, est dans l'intervalle $[l,h]$. On veut connaître la précision nécessaire pour représenter les résultats. Précisions nécessaires pour les résultats d'une fonction $F$ sur un interval $[l,h]$ et avec une précision $p$
$$
\mathrm{P}(F,l,h,p)=\{\}
$$
