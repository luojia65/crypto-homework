#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/sm3.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/sm2.h>
#include <openssl/pkcs7.h>
using namespace std;

/*const char* cacert = "C:\\Users\\Jang\\Desktop\\Crypto\\GMSSL\\sample\\cacert.txt";
const char* Bcert = "C:\\Users\\Jang\\Desktop\\Crypto\\GMSSL\\sample\\UserB.crt";
const char* pkfile = "C:\\Users\\Jang\\Desktop\\Crypto\\GMSSL\\sample\\pkey.key";
const char* datafile = "C:\\Users\\Jang\\Desktop\\Crypto\\GMSSL\\sample\\data.pem";*/
char result[10000];

const char cacert[] = "\
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
-----END CERTIFICATE----- \n\
";
const char pkeyB[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEINQhCKslrI3tKt6cK4Kxkor/LBvM8PSv699Xea7kTXTToAoGCCqBHM9V \n\
AYItoUQDQgAEH7rLLiFASe3SWSsGbxFUtfPY//pXqLvgM6ROyiYhLkPxEulwrTe8 \n\
kv5R8/NA7kSSvcsGIQ9EPWhr6HnCULpklw== \n\
-----END EC PRIVATE KEY----- \n\
";

X509* getX509(const char* cert)
{
	BIO* bio;
	bio = BIO_new(BIO_s_mem());
	BIO_puts(bio, cert);
	return  PEM_read_bio_X509(bio, NULL, NULL, NULL);
}
EVP_PKEY* getpkey(const char* private_key)
{
	BIO* bio_pkey = BIO_new_mem_buf((char*)private_key, strlen(private_key));
	if (bio_pkey == NULL)
		return NULL;
	return  PEM_read_bio_PrivateKey(bio_pkey, NULL, NULL, NULL);
}

bool checkCanSign(X509* pX509Cert)
{
	ASN1_BIT_STRING* lASN1UsageStr;
	lASN1UsageStr = (ASN1_BIT_STRING*)X509_get_ext_d2i(pX509Cert, NID_key_usage, NULL, NULL);
	//X509v3_KU_DIGITAL_SIGNATURE
	if (lASN1UsageStr)
	{
		unsigned short usage = lASN1UsageStr->data[0];
		if (lASN1UsageStr->length > 1)
		{
			usage |= lASN1UsageStr->data[1] << 8;
		}
		if (usage & X509v3_KU_DIGITAL_SIGNATURE)
			return true;
	}
	return false;
}

int solve(char *datas)
{
	OpenSSL_add_all_algorithms();

	BIO* cabio = BIO_new_mem_buf(cacert, strlen(cacert));
	X509* root = PEM_read_bio_X509(cabio, NULL, NULL, NULL);
	EVP_PKEY* pubkey = X509_get_pubkey(root);

	//读p7
	BIO* bio_data = BIO_new_mem_buf(datas, strlen(datas));
	if (bio_data == NULL)
		return 0;
	PKCS7* p7 = PEM_read_bio_PKCS7(bio_data, NULL, NULL, NULL);
	if (p7 == NULL)
		return 0;

	STACK_OF(X509)* certs = p7->d.signed_and_enveloped->cert;
	for (int i = 0; i < sk_X509_num(certs); i++) {
		X509* certx = sk_X509_value(certs, i);
		if (X509_verify(certx,pubkey) != 1)
			return 0;
	}

	EVP_PKEY* pKey = getpkey(pkeyB);
	BIO* bio_out = PKCS7_dataDecode(p7,pKey,NULL,NULL);
	int len = BIO_read(bio_out, result, 10000);

	//验签
	STACK_OF(PKCS7_SIGNER_INFO)* sk = PKCS7_get_signer_info(p7);
	int signCount = sk_PKCS7_SIGNER_INFO_num(sk);
	for (int i = 0; i<signCount;i++)
	{
		//获得签名者信息
		PKCS7_SIGNER_INFO * signInfo = sk_PKCS7_SIGNER_INFO_value(sk,i);
		//获得签名者证书
		//X509 * cert = PKCS7_cert_from_signer_info(p7,signInfo);
		//X509* cert = getX509(certC);
		//X509* cert = sk_X509_value(certs, 1);
		X509* cert = X509_find_by_issuer_and_serial(p7->d.signed_and_enveloped->cert, signInfo->issuer_and_serial->issuer, signInfo->issuer_and_serial->serial);
		//检查能否签名
		if (!checkCanSign(cert))
			return 0;
		//验证证书
		if (X509_verify(cert,pubkey) != 1)
			return 0;
		//验证签名
		if (PKCS7_signatureVerify(bio_out,p7,signInfo,cert) != 1)
			return 0;
	}
	return len;
}

char oneline[1005];
char inbuf[100005];

int main(void)
{
	//freopen("C:\\Users\\Jang\\Desktop\\Crypto\\GMSSL\\input\\1.in", "r", stdin);
	inbuf[0] = 0;
	while (!cin.eof()) {
		cin.getline(oneline, 1000);
		strcat(inbuf, oneline);
		strcat(inbuf, "\n");
	}
	int ret=solve(inbuf);
	if (ret == 0)
		cout << "ERROR" << endl;
	else {
		for (int i = 0; i < ret; i++) {
			putchar(result[i]);
		}
		putchar('\n');
	}
	return 0;
}