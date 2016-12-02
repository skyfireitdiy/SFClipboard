#ifndef SFAES_H_
#define SFAES_H_
#include <QObject>
#include <QByteArray>

class SFAES :public QObject
{
    Q_OBJECT
public:
    //SFAES(unsigned char* key,QObject *parent=0);
    SFAES(QString key, QObject *parent=0);
    //SFAES(QByteArray key, QObject *parent=0);
    virtual ~SFAES();
    QByteArray Encrypt(QByteArray input)const;
    QByteArray Decrypt(QByteArray input, int ret_len=-1)const;

private:
	unsigned char Sbox[256];
	unsigned char InvSbox[256];
	unsigned char w[11][4][4];

	void KeyExpansion(unsigned char* key, unsigned char w[][4][4]);
    unsigned char FFmul(unsigned char a, unsigned char b) const;

    void SubBytes(unsigned char state[][4]) const;
    void ShiftRows(unsigned char state[][4]) const;
    void MixColumns(unsigned char state[][4]) const;
    void AddRoundKey(unsigned char state[][4], const unsigned char k[][4]) const;

    void InvSubBytes(unsigned char state[][4])const;
    void InvShiftRows(unsigned char state[][4]) const;
    void InvMixColumns(unsigned char state[][4])const;

    unsigned char* Cipher(unsigned char* input)const;
    unsigned char* InvCipher(unsigned char* input)const;
    void* Cipher(void* input, int length=0)const;
    void* InvCipher(void* input, int length)const;

};

#endif
