# VDF

## Introduction
Today I will present the work Alessio and I did on Verifiable Delay Function, focusing in particular on Benjamin Wesolowski's paper written here.

I will first present the general idea behind VDF's, then explain in more detail the construction from 

- What is a VDF and why do we want to use them 
- Wesolowski construction
- Security of the VDF
- Implementation and results
- 


## Slow Funtions and their usefullness

The goal is to create a function f of X into Y, that takes a certain amount of time Delta to execute, and cannot be computed faster even when using multpile processors. 

Practically, this would mean that f takes say X minutes to compute.

This concept has applications in blockchains as well as random beacons. 


Imagine a scenario where randomness for a public lottery is source from k stocks, and the output of which is 0 or 1 depending on whether the stock closed positive or negative for the day. This produces a $2^k$ bit number. The attacker could simulate the $2^k$ outcomes and then choose the one the wishes by last minute trading. Using a delay of 1 hour say, this would become unfeasible because the stocks will have already closed.


We could use a slow hash function or a many iterations of it,  but the only way to verify this is by doing the comuptation again
Thus we require the ouput to be publicly verifiable by any 3rd party in time polylog(Delta). 

We want this function to be secure in the sense that we cannot cheat in any way. This means it is impossible to calculate the output faster than Delta and that incorrect outputs will not pass verification. We will detail these later.


## time lock puzzle 

$x^{2^t} mod N$ takes t squarings if $N=pq$ is unkown. 

One way of constructing such functions relies on very simple group theory and inspired by the Rivest Shamir Wagenr time lock puzzle. The idea is that performing g^{2^t} within a group of finite order takes at least t squarings when the order of G is unknown. 

We assume like in the time lock puzzle that it is impossible to calculate this faster than performing t squarings, unless the order is known.


## Complexity

We will use different complexity classes to describe the algorithms
Security parameter k = 128

RSA key size must be at least 4096


EGCD s,t st $as+bt = \gcd(a,b)$ $\mathcal{O}(\log_2a \cdot \log_2b)$

GCD $\gcd(a,b)$ $\mathcal{O}(\log_2a \cdot \log_2b)$

$a \bmod N$ $\mathcal{O}(\log_2N \cdot \log_2 \frac{a}{N} )$

$(a \bmod N)(b \bmod N) = a\cdot b \bmod N$  $\mathcal{O}(\log N)$ multiplication


In $\mathbb{Z}/N\mathbb{Z}$:

$g \cdot h$ $\mathcal{O}(\log^2_2N )$

$g^e$ $\mathcal{O}(\log_2e \cdot \log^2_2N )$



## Definition

We can now properly define a VDF as a set of 3 algorithms setup eval and verify. 

###Setup(k,Delta)
Takes as input a security parameter k, and timing parameter Delta and outputs some public parameter pp
k determines the security of our setup, and is used to define security of hash functions as well as the group G, so that a cryptographic attack on the setup would be infeasible with current or future hardware.

### pp 
- $G$ The abelian group in which we will the calculations are performed in. If we are working with RSA groups, this would be the size of $p,q$ and can be found on keylength. We use 4096 as minimum but could use more. For class groups, this depends on finding the hardness of finding the order 
- $H_{prime}: G \times G \rightarrow  Primes(2k)$ 
- $H_G : \mathcal{X} \rightarrow G$
- $\delta$ Time necessary to compute 1 squaring in $G$, usually thought out to be the practically fastest way, even using specialized hardware
- $t$ The number of squarings necessary to acheive computation time of $\Delta$, $\lceil \Delta / \delta \rceil$
- $\Delta$ 
- $k$ 

In our case, the public parameters all depend implicitly on k. Some of these parameters could then be published on a blockchain for anyone to use 


### evalpp(x) -> (y,pi) 
is the output of our slow function. It takes Delta time to compute y = H(G)^2t since there are t squarings. usually, t is about 2^30
the proof is calculated later,

The reason for hasing $x \in \mathcal{X}$ is to remove the homomrphism property of $g \mapsto y=g^{2^t}$. If $h = g^\alpha$ then $h \mapsto h^{2^t}=(g^\alpha)^{2^t} = (g^{2^t})^\alpha = y^\alpha$.

We will not go into further details about this hash function because I have not studied them enough to understand their properties, but we note that the hashing must into a large enough group that precomputations cannot be made, and that the output could then be found in a faster way.

### verify(x,y,pi) -> True False 
this function decides whether the output is the correct one for input x . should run in time $polylog(\Delta)$



## Properties 
### Soundness 
This is the property that it is not possible to find any other solution (x,y,pi) which satisfy verify = true for arbitrary x. Otherwise, attackers could bypass eval and break trust in the setup

### Sequentiality 
There is no algorithm running on at most polyk processors which can perform eval faster than t sequential squarings. The idea is that the only other way would be to calculate many values at once, and whenever we find some sequence we have already computed, we can switch to it.

### epslion evaluation 

The total eval procedure must take at most $(1+\epsilon)\Delta$ to compute where 
$\epsilon\Delta$ is the time required to compute the proof.
In our case, the squaring step takes O(t) group operations, and we will show that the proof can be generated in O(t/logt) group ops. 

 
This is just a property to make sure we dont take too long to produce the proof

## Trapdoor
A trapdoor VDF is a VDF with the additional property that there exists a private key sk such that trapdoor_pp(sk,x) is faster than eval

When working in a group, sk represents the order of the group G say M.
$g^{2^t} = g^{2^t \mod M}$ So we perform 

This can be computed then 

## RSA-VDF
### Setup
The 



## Problems

The construciton relies on the unknown order of group which can be broken by quantum computers. Thus we would need to find another structure than the algebraic one just described.



## Further work


### Understanding hash functions and how they can break the setup

### Working with algebra on computers 
Working with C was a very interesting challenge. I had already written a heart-rate detection algorithm so was aware of all the challenges the language poses. The advantage of course is the closeness to the hardware, and the control we have over what is executed. This leads however to a lot of messy and difficult to understand code, which then leads to hard to diagnos bugs.

When I started the research I discovered an implementation written in Rust, which supposedly has many of the efficiency properties of C, while incorporating many new language safety features. I also explored Swift, the new language by Apple and have been exploring how to implement the VDF in another language with the goal of benchmarking them and seeing whether there are advantages in working with other languages.

Having studied a lot of algebra, I was stunned to see how the simple concrete condstruction of Z/NZ gave me acces to the many propoerties I had already studied, which remain hidden from the implementation.


The idea would be to create a framework for VDF which could work in any setup 

### Class groups
The use of the RSA-VDF group is not very practical due to the fact that it is a Trapdoor-VDF. Using class groups within this setting would provide better trust in the setup and again would be interesting to explore. 

If given more time, I would love to program using groups in a more mathematical way, and abstract their implementation from the actual VDF, using newer programming language such as swift. 

