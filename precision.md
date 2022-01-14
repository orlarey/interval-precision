# Détermination de la précision nécessaire

## Troncature

Pour $x\in\mathbb{R}$ et $p\in\mathbb{Z}$, on définit une opération de troncature, notée $x_p$, qui consiste à ne garder que les $p$ premiers bits après le virgule de $x$ : 
$$
x_p = \frac{\lfloor x*2^p \rfloor}{2^p}
$$
Remarque : si $p$ est négatif, cela revient à remplacer par des zéros les $-p$ bits avant la virgule.

## Troncature majorée

Pour $x\in\mathbb{R}$ et $p\in\mathbb{Z}$, on définit une opération de troncature majorée, notée $\overline{x_p}$  : 
$$
\overline{x_p} = \frac{\lfloor x*2^p+1 \rfloor}{2^p}
$$

Propriété : $x_p<\overline{x_p}$

## Intervalle de troncature

A tout nombre tronqué $x_p$ on peut associer un intervalle  :  
$$
\mathrm{Interval}(x_p)=[x_p,\overline{x_p}[
$$
contenant tous les réels de même troncature que $x$ à $p$ bits, de sorte que : 
$$
\forall u\in[x_p,\overline{x_p}[\Leftrightarrow  u_p=x_p
$$

## Plus grande précision commune

Pour un intervalle $[l,h[$ on peut calculer une *plus grande précision commune* à toutes les valeurs de l'intervalle $p\in\mathbb{Z}$ telle que :
$$
\mathrm{pgpc}([l,h[) = p\mid\forall u,v \in [l,h[, (u_p=v_p) \and (u_{p+1}\neq v_{p+1})
$$

## Découpage d'un intervalle

Tronquer les valeurs d'un intervalle 
$$
[l,h]_p = \{x_p \mid x\in [l,h]\}
$$

## Précision nécessaire

Soit une fonction $F(x)$ dont l'argument $x$, exprimé avec une précison $p$, est dans l'intervalle $[l,h]$. On veut connaitre la précisions nécessaires pour représenter les résultats. Précisions nécessaires pour les résultats d'une fonction $F$ sur un interval $[l,h]$ et avec une précision $p$
$$
\mathrm{P}(F,l,h,p)=\{\}
$$
