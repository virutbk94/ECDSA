#include <iostream>
#include <fstream>
#include <cstring>
#include <NTL/ZZ.h>
#include "convert.h"
#include "sha.h"

using namespace std;
using namespace NTL;

struct point_s
{
    ZZ x;
    ZZ y;
    bool inf;
};

typedef struct point_s point;

struct curve_s
{
    ZZ p;
    ZZ a;
    ZZ b;
    point G;
    ZZ n;
    ZZ h;
};

typedef struct curve_s curve;

struct signature_s
{
    ZZ r;
    ZZ s;
};

typedef struct signature_s signature;

static curve E;
static ZZ privateKey;
static point publicKey;
static signature sig;
static char* data;


void print_point(point P)
{
    if(P.inf)
    {
        cout<<"This is infinity"<<endl;
    }
    else
    {
        char* temp = (char*)malloc(65);
        conv_ZZ_to_hex(temp,P.x,64);
        cout<<"x = "<<temp<<endl;
        conv_ZZ_to_hex(temp,P.y,64);
        cout<<"y = "<<temp<<endl;
        free(temp);
    }
}

void print_curve(curve E)
{
    char* temp = (char*)malloc(65);
    conv_ZZ_to_hex(temp,E.p,64);
    cout<<"p = "<<temp<<endl;
    cout<<"E: y^2 = x^3 + "<<E.a<<"*x + "<<E.b<<endl;
    print_point(E.G);
    conv_ZZ_to_hex(temp,E.n,64);
    cout<<"n = "<<temp<<endl;
    cout<<"h = "<<E.h<<endl;
    free(temp);
}

void print_privateKey()
{
    char* temp = (char*)malloc(65);
    conv_ZZ_to_hex(temp,privateKey,64);
    cout<<"primary key = "<<temp<<endl;
    free(temp);
}


void print_publicKey()
{
    cout<<"Public key :"<<endl;
    print_point(publicKey);
}

void print_signature()
{
    cout<<"Signature :"<<endl;
    char* temp = (char*)malloc(65);
    conv_ZZ_to_hex(temp,sig.r,64);
    cout<<"r = "<<temp<<endl;
    conv_ZZ_to_hex(temp,sig.s,64);
    cout<<"s = "<<temp<<endl;
    free(temp);
}

void print_data()
{
    cout<<"Data: "<<data<<endl;
}

void ECDSA();
void TaoKhoa();
void Ky();
void XacThuc();

bool load_curve(char* path);
bool load_privateKey(char* path);
bool load_publicKey(char* path);
bool load_data(char* path);
bool load_signature(char* path);

bool save_privateKey(char* path);
bool save_publicKey(char* path);
bool save_signature(char* path);

bool compute_publicKey();
bool generate_signature();
bool check_signature();

void double_point(point& a,point b);
void add_point(point& c,point a,point b);
void multi_point(point& a,ZZ k,point b);
void inv_point(point& a,point b);
void copy_point(point& a,point b);
bool cmp_point(point a,point b);

int main()
{
    data = (char*)malloc(65);
    ECDSA();
    free(data);
    return 0;
}

void ECDSA()
{
    char* chon = (char*)malloc(50);
    bool check = true;
    while(check)
    {

        check = false;
        cout<<"Chon chuc nang"<<endl<<"1.Tao Khoa"<<endl<<"2.Ky len ban tin"<<endl<<"3.Xac thuc chu ky"
            <<endl<<"4.Ket thuc"<<endl;
        cin>>chon;
        cin.ignore();
        if(strcmp(chon,"1") == 0)
        {
            TaoKhoa();
            check=true;
        }
        else if(strcmp(chon,"2")== 0)
        {
            Ky();
            check=true;
        }
        else if(strcmp(chon,"3") == 0)
        {
            XacThuc();
            check=true;
        }
        else if(strcmp(chon,"4")== 0)
        {
            system("exit");
        }
        else
        {
            check = true;
        }
    }
    free(chon);
}

void TaoKhoa()
{
    char* path = (char*)malloc(50);
    bool success = false;
LOAD_E:
    cout<<"Load duong cong Elliptic: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_curve(path);
    if(!success) goto LOAD_E;
    print_curve(E);

    cout<<"Nhap khoa bi mat: "<<endl;
    cin>>privateKey;
    print_privateKey();

    cout<<"Tinh khoa cong khai"<<endl;
    success = compute_publicKey();
    print_publicKey();

    cout<<"Luu khoa bi mat: "<<endl;
    cin>>path;
    cin.ignore();

    success = save_privateKey(path);
    cout<<"Luu khoa cong khai: "<<endl;
    cin>>path;
    cin.ignore();
    success = save_publicKey(path);

    free(path);
}

void Ky()
{
    bool success;
    char *path = (char*)malloc(50);
LOAD_E:
    cout<<"Load duong cong Elliptic: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_curve(path);
    if(!success) goto LOAD_E;
    print_curve(E);

LOAD_PRK:
    cout<<"Load khoa bi mat: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_privateKey(path);
    if(!success) goto LOAD_PRK;
    print_privateKey();

LOAD_DATA:
    cout<<"Load va bam du lieu: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_data(path);
    if(!success) goto LOAD_DATA;
    print_data();

    cout<<"Tao chu ky dien tu"<<endl;
    success = generate_signature();
    print_signature();
    cout<<"Luu chu ky dien tu: "<<endl;
    cin>>path;
    cin.ignore();
    success = save_signature(path);
    free(path);
}

void XacThuc()
{
    bool success;
    char *path = (char*)malloc(50);
LOAD_E:
    cout<<"Load duong cong Elliptic: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_curve(path);
    if(!success) goto LOAD_E;
    print_curve(E);

LOAD_PBK:
    cout<<"Load khoa cong khai: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_publicKey(path);
    if(!success) goto LOAD_PBK;
    print_publicKey();

LOAD_SIG:
    cout<<"Load chu ky dien tu: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_signature(path);
    if(!success) goto LOAD_SIG;
    print_signature();

LOAD_DATA:
    cout<<"Load du lieu: "<<endl;
    cin>>path;
    cin.ignore();
    success = load_data(path);
    if(!success) goto LOAD_DATA;
    print_data();

    cout<<"Kiem tra chu ky"<<endl;
    success = check_signature();
    if(success) cout<<"Xac Thuc"<<endl;
    else cout<<"Khong xac thuc"<<endl;

    free(path);
}

bool load_curve(char* path)
{
    char* temp = (char*)malloc(65);
    ifstream in;
    in.open(path);
    if(!in.is_open())
    {
        cout<<"Khong mo duoc file"<<endl;
        return false;
    }
    else
    {
        in.getline(temp,65);
        conv_hex_to_ZZ(E.p,temp);
        in.getline(temp,65);
        conv_hex_to_ZZ(E.a,temp);
        in.getline(temp,65);
        conv_hex_to_ZZ(E.b,temp);
        in.getline(temp,65);
        conv_hex_to_ZZ(E.G.x,temp);
        in.getline(temp,65);
        conv_hex_to_ZZ(E.G.y,temp);
        E.G.inf = false;
        in.getline(temp,65);
        conv_hex_to_ZZ(E.n,temp);
        in.getline(temp,65);
        conv_hex_to_ZZ(E.h,temp);
    }
    in.close();
    free(temp);
    return true;
}

bool load_data(char* path)
{
    return sha_256(path,data);
}

bool load_privateKey(char* path)
{
    ifstream in;
    char* temp = (char*) malloc(65);
    in.open(path);
    if(!in.is_open())
    {
        cout<<"Khong mo duoc file"<<endl;
        return false;
    }
    else
    {
        in.getline(temp,65);
        conv_hex_to_ZZ(privateKey,temp);
    }
    free(temp);
    in.close();
    return true;
}

bool load_publicKey(char* path)
{
    char * temp = (char*)malloc(65);
    ifstream in;
    in.open(path);
    if(!in.is_open())
    {
        cout<<"Khong mo duoc file"<<endl;
        return false;
    }
    else
    {
        in.getline(temp,65);
        conv_hex_to_ZZ(publicKey.x,temp);
        in.getline(temp,65);
        conv_hex_to_ZZ(publicKey.y,temp);
    }
    free(temp);
    in.close();
    return true;
}

bool load_signature(char* path)
{
    char *temp = (char*)malloc(65);
    ifstream in;
    in.open(path);
    if(!in.is_open())
    {
        cout<<"Khong mo duoc file"<<endl;
        return false;
    }
    else
    {
        in.getline(temp,65);
        conv_hex_to_ZZ(sig.r,temp);
        in.getline(temp,65);
        conv_hex_to_ZZ(sig.s,temp);
    }
    in.close();
    free(temp);
    return true;
}

bool save_privateKey(char* path)
{
    ofstream out;
    out.open(path,ios::out|ios::trunc);
    if(!out.is_open())
    {
        cout<<"Error"<<endl;
        return false;
    }
    else
    {
        char* a = (char*)malloc(65);
        conv_ZZ_to_hex(a,privateKey,64);
        out<<a;
        free(a);
    }
    out.close();
    return true;
}

bool save_publicKey(char* path)
{
    ofstream out;
    out.open(path,ios::out|ios::trunc);
    if(!out.is_open())
    {
        cout<<"Error"<<endl;
        return false;
    }
    else
    {
        char* a = (char*)malloc(65);
        conv_ZZ_to_hex(a,publicKey.x,64);
        out<<a<<endl;
        conv_ZZ_to_hex(a,publicKey.y,64);
        out<<a<<endl;
        free(a);
    }
    out.close();
    return true;
}

bool save_signature(char* path)
{
    ofstream out;
    out.open(path,ios::out|ios::trunc);
    if(!out.is_open())
    {
        cout<<"Error"<<endl;
        return false;
    }
    else
    {
        char* a = (char*)malloc(65);
        conv_ZZ_to_hex(a,sig.r,64);
        out<<a<<endl;
        conv_ZZ_to_hex(a,sig.s,64);
        out<<a<<endl;
        free(a);
    }
    out.close();
    return true;
}

bool compute_publicKey()
{
    ZZ k = privateKey%E.n;
    multi_point(publicKey,k,E.G);
    return true;
}

bool generate_signature()
{
    point Q;
    ZZ n = E.n;

    //chon k ngau nhien 2 -> n-1
BUOC_1:
    ZZ k = RandomLen_ZZ(256)%(n -2) + 2;
    //Tinh Q = kG (x1,y1)
    multi_point(Q,k,E.G);
    //Tinh r = x1 mod n
    sig.r = Q.x%n;
    //Neu r = 0 quay lai buoc 1
    if(sig.r==0) goto BUOC_1;
    else
    {
        //tinh s = k^-1 * (m + d*r) mod n;
        ZZ m;
        conv_hex_to_ZZ(m,data);
        sig.s = MulMod(InvMod(k%n,n),m + privateKey * sig.r,n);
        if(sig.s == 0) goto BUOC_1;
    }
    return true;
}

bool check_signature()
{
    ZZ r = sig.r;
    ZZ s = sig.s;
    ZZ n = E.n;
    ZZ m;
    conv_hex_to_ZZ(m,data);
    point G,Q;
    copy_point(G,E.G);
    copy_point(Q,publicKey);

    if(r>=2 && r<n && s>=2 && s<n)
    {
        //Tinh w = s^-1 mod n
        ZZ w = InvMod(s,n);
        //Tinh u1 = mw mod n
        ZZ u1 = MulMod(m,w,n);
        //tinh u2 = rw mod n
        ZZ u2 = MulMod(r,w,n);

        point X,X1,X2;
        // Tinh X = u1*G + u2*Q
        multi_point(X1,u1,G);
        multi_point(X2,u2,Q);
        add_point(X,X1,X2);

        if(X.inf)
        {
            return false;
        }
        else
        {
            ZZ v = X.x%n;
            if(v==r) return true;
        }
    }
    return false;
}

//C = A + B
void add_point(point& c,point a,point b)
{
    ZZ p = E.p;

    // neu b = inf thi c = a
    if(b.inf)
    {
        copy_point(c,a);
    }
    // neu a = inf thi c = b
    else if(a.inf)
    {
        copy_point(c,b);
    }
    //neu a = b thi c = 2*a
    else if(cmp_point(a,b))
    {
        double_point(c,a);
    }
    // neu a != b != inf
    else
    {
        point temp;
        inv_point(temp,a);
        // new b = -a thi c = inf
        if(cmp_point(temp,b))
        {
            c.inf = true;
        }
        // new b!= -a
        else
        {
            // lamda = (yB - yA)/(xB - xA) mod p
            ZZ lamda;
            lamda = MulMod((b.y%p - a.y%p),InvMod((b.x%p - a.x%p)%p,p),p);
            // xC = (lamda^2 - xA -xB) mod p
            c.x = (power(lamda,2)%p - a.x%p - b.x%p)%p;
            //yC = lamda*(xA - xC) - yA mod p
            c.y = (lamda*(a.x%p - c.x%p)%p - a.y%p)%p;
            c.inf = false;
        }
    }
}

//A = 2B
void double_point(point& a,point b)
{
    ZZ p = E.p;
    if(b.inf)
    {
        a.inf = true;
    }
    else if(b.y == 0)
    {
        a.inf = true;
    }
    else
    {
        // lamda = (3*xB^2 + a)/2*yB
        ZZ lamda;
        lamda = MulMod((3*power(b.x,2)%p + E.a%p)%p,InvMod(2*b.y%p,p),p);
        // xC = (lamda^2 - xA -xB) mod p
        a.x = (power(lamda,2)%p - b.x%p - b.x%p)%p;
        //yC = lamda*(xA - xC) - yA mod p
        a.y = (lamda*(b.x%p - a.x%p)%p - b.y%p)%p;
        a.inf = false;
    }
}

//A = kB
void multi_point(point& a,ZZ k,point b)
{
    if(b.inf)
    {
        a.inf = true;
        return;
    }

    point T0,T1;
    copy_point(T0,b);
    copy_point(T1,T0);

    while(k>to_ZZ(1))
    {
        if(k%2 == 0)
        {
            add_point(T1,T0,T1);
            double_point(T0,T0);
            k=k/2;

        }
        else
        {
            double_point(T0,T0);
            k=k/2 + 1;
        }
    }
    copy_point(a,T1);
}

void copy_point(point& a,point b)
{
    a.x = b.x;
    a.y = b.y;
    a.inf = b.inf;
}


bool cmp_point(point a,point b)
{
    if(a.inf == b.inf && a.x == b.x && a.y == b.y)
        return true;
    else
        return false;
}

void inv_point(point& a,point b)
{
    a.inf = b.inf;
    if(!a.inf)
    {
        a.x = b.x;
        a.y = -b.y;
    }
}
