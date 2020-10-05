#include <iostream>
#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/pkcs7.h>
#include <openssl/x509.h>
#include <openssl/sm2.h>
#include <openssl/rand.h>
#include <openssl/objects.h>
#include <openssl/x509v3.h>
 
#define FOR(x,f,t) for(x=f;x<=t;++x)
using namespace std;

char CA[]="\
-----BEGIN CERTIFICATE----- \n\
MIIB/zCCAaagAwIBAgIJAKKa0PAt9M1FMAoGCCqBHM9VAYN1MFsxCzAJBgNVBAYT \n\
AkNOMQ4wDAYDVQQIDAVIdUJlaTEOMAwGA1UEBwwFV3VIYW4xDTALBgNVBAoMBEhV \n\
U1QxDDAKBgNVBAsMA0NTRTEPMA0GA1UEAwwGY2Fyb290MB4XDTIwMDkyMDIwNTkx \n\
OVoXDTMwMDkxODIwNTkxOVowWzELMAkGA1UEBhMCQ04xDjAMBgNVBAgMBUh1QmVp \n\
MQ4wDAYDVQQHDAVXdUhhbjENMAsGA1UECgwESFVTVDEMMAoGA1UECwwDQ1NFMQ8w \n\
DQYDVQQDDAZjYXJvb3QwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAASJ8mm28JJR \n\
bZKLr6DCo1+KWimpKEsiTfZM19Zi5ao7Au6YLosyN71256MWmjwkwXxJeLa0lCfm \n\
kF/YWCX6qGQ0o1MwUTAdBgNVHQ4EFgQUAL5hW3RUzqvsiTzIc1gUHeK5uzQwHwYD \n\
VR0jBBgwFoAUAL5hW3RUzqvsiTzIc1gUHeK5uzQwDwYDVR0TAQH/BAUwAwEB/zAK \n\
BggqgRzPVQGDdQNHADBEAiAaZMmvE5zzXHx/TBgdUhjtpRH3Jpd6OZ+SOAfMtKxD \n\
LAIgdKq/v2Jkmn37Y9U8FHYDfFqk5I0qlQOAmuvbVUi3yvM= \n\
";

char pkeyB[]="\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEINQhCKslrI3tKt6cK4Kxkor/LBvM8PSv699Xea7kTXTToAoGCCqBHM9V \n\
AYItoUQDQgAEH7rLLiFASe3SWSsGbxFUtfPY//pXqLvgM6ROyiYhLkPxEulwrTe8 \n\
kv5R8/NA7kSSvcsGIQ9EPWhr6HnCULpklw== \n\
-----END EC PRIVATE KEY----- \n\
";

char input[10005],ans[10005];

// source: pkcs7-gen.cpp
X509 *getX509(const char *cert)
{
	BIO *bio;
	bio = BIO_new(BIO_s_mem());
	BIO_puts(bio, cert);
    return PEM_read_bio_X509(bio, NULL, NULL, NULL);
}
// source: pkcs7-gen.cpp
EVP_PKEY *getpkey(const char *private_key)
{
	BIO *bio_pkey = BIO_new_mem_buf((char *)private_key, strlen(private_key));
	if (bio_pkey == NULL)
 		return NULL;
 	return PEM_read_bio_PrivateKey(bio_pkey, NULL, NULL, NULL);
}
int main() {
    // 初始化
	OpenSSL_add_all_algorithms();
    // 1.读入所有数据
    char c;int l=0; while(c=getchar(),c!=EOF) input[l++]=c;
    input[l]='\0';
    // 2.得到BIO类型
	BIO *bin = BIO_new_mem_buf(input,l);
    if(bin == NULL) { puts("ERROR"); return 0; }
    // 3.得到PKCS7类型
	PKCS7 *p7 = PEM_read_bio_PKCS7(bin,NULL,NULL,NULL);
    if(p7 == NULL) { puts("ERROR"); return 0; }
    // 读根证书
    BIO *bca = BIO_new_mem_buf(CA,strlen(CA));
    printf("bca: %p\n",bca);
    X509 *root = PEM_read_bio_X509(bca,NULL,NULL,NULL);
    printf("root: %p\n",root);
    EVP_PKEY *pubKey = X509_get_pubkey(root);
    printf("pubkey: %p\n",pubKey);
    // 验签（输入的数据是否由证书发行）
	STACK_OF(X509)* crts=p7->d.signed_and_enveloped->cert;
    int i;
	FOR(i,0,sk_X509_num(crts)-1) {
		X509* crt = sk_X509_value(crts, i);
        printf("%d %p\n",i,crt);
		if (X509_verify(crt, pubKey) != 1) { puts("ERROR"); return 0; }
	}
    // 4.解码
	EVP_PKEY *pKey = getpkey(pkeyB);
    BIO *bout = PKCS7_dataDecode(p7,pKey,NULL,NULL);
    if(bout == NULL) { puts("ERROR"); return 0; }
    // 验证解码
	STACK_OF(PKCS7_SIGNER_INFO)* sk = PKCS7_get_signer_info(p7);
    FOR(i,0,sk_PKCS7_SIGNER_INFO_num(sk)-1) {
		PKCS7_SIGNER_INFO *info = sk_PKCS7_SIGNER_INFO_value(sk,i);
        X509 *scrt=X509_find_by_issuer_and_serial(
            p7->d.signed_and_enveloped->cert, 
            info->issuer_and_serial->issuer, 
            info->issuer_and_serial->serial
        );
        if (X509_verify(scrt, pubKey) != 1) { puts("ERROR"); return 0; }
        if (PKCS7_signatureVerify(bout,p7,info,scrt) != 1) { puts("ERROR"); return 0; }
    }
    // 5.得到输出的缓冲区
    int olen = BIO_read(bout,ans,10005);
    printf("%s\n",ans);
    return 0;
}
