#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>

#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>

typedef unsigned long long ull;
typedef long long ll;

// Calculate mod with negatives
ll mod (ll a, ll n) {
    return a < 0 ? n - (-a) % n : a % n;
}

// Calculate the modular inverse of b mod a
// Return -1 if a, b aren't coprimes.
// O(log(min(a, b)))
inline
ll modular_inverse(ll a, ll b){
    // ll s = 0, s0 = 1;
    ll t = 1, t0 = 0;
    ll r = b, r0 = a;
    ll aux, q = 0;

    while (r) {
        q = r0 / r; 

        aux = r;
        r = r0 - q * r;
        r0 = aux;

        // aux = s;
        // s = s0 - q * s;
        // s0 = s;

        aux = t;
        t = t0 - q * t;
        t0 = aux;
    }

    // Bezeout coefficients: s, t | a*s + b*t = gcd(a,b)
    // Thus, t0 is the b^-1 mod a if gcd(a,b) = 1
    // Source and explanation: https://es.wikipedia.org/wiki/Algoritmo_de_Euclides#Algoritmo_de_Euclides_extendido

    return r0 != 1 ? -1 : mod (t0, a);
}

// Get prime factors of n, modified to the problem
auto find_prime_factors(ull n){

    auto r = std::vector<ull>();

    // Separated for optimization
    if (!(n & 1))
        r.push_back(2);
    while (!(n & 1))
        n >>= 1;

    for (ull i = 3; i * i <= n; ++i){

        if (n % i == 0)
            r.push_back(i);
        while (n % i == 0)
            n /= i;

    }

    if (n > 2)
        r.push_back(n);

    return std::tuple(r[0], r[1]);

}

// Calculate d given (e, n)
auto breaking_rsa(ull e, ull n) {

    std::cout << "Using e: " << e << "\nUsing n: " << n << "\n\n";

    auto [p, q] = find_prime_factors(n);
    std::cout << "p & q: " << p << " " << q <<"\n";

    auto o = (p - 1) * (q - 1);
    std::cout << "o(n): " << o << "\n";

    return std::tuple(modular_inverse(o, e), o);

}

// Check is e * d mod o is equals to 1
void validate(ull e, ull d, ull o) {

    CryptoPP::Integer E = e, D = d, O = o;

    std::cout << "Validating: e * d mod o(n) = " << a_times_b_mod_c(E, D, O) << "\n";

}

int main(void){

    int test;
    std::cin >> test;

    ull e, n;
    std::string name;

    while (test --> 0){
        std::cout << "==============\n";
        std::cin >> name;
        std::cout << "Classmate's name: " << name << "\n";

        std::cin >> e >> n;
        auto [d, o] = breaking_rsa(e, n);
        std::cout << "d: " << d << "\n";

        validate(e, d, o);
    }

    return 0;

}