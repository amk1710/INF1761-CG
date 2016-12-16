/* 
 * File:   Pixel.h
 * Author: jcoelho
 *
 * Created on September 11, 2016, 6:03 PM
 */

#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
    /**
     * Construtor default.
     */
    Pixel( );

	/**
	* Construtor 3xfloat.
	*/

	Pixel(float r, float g, float b);

    /**
     * Sobrecarga do operador [] para acessar as informacoes dos pixels.
     * @param i - posicao a ser acessada.
     * @return - referencia para a posicao.
     */
    float operator[]( unsigned int i ) const;

    /**
     * Sobrecarga do operador [] para acessar as informacoes dos pixels.
     * @param i - posicao a ser acessada.
     * @return - referencia para a posicao.
     */
    float& operator[]( unsigned int i );

    /**
     * Realiza o "produto cor" entre dois pixels.
     * @param p - pixel a ser multiplicado pelo pixel corrente.
     * @return - resulatado do produto escalar.
     */
    Pixel operator*( const Pixel& p ) const;

	/**
	* Multiplica o pixel pelo escalar k
	* @param p - pixel a ser multiplicado por k.
	* @return - resultado do produto escalar.
	*/
	Pixel operator*(const float& k) const;

    /**
     * Sobrecarga do operador -.
     * @param p - pixel a ser subtraido.
     * @return - resultado da operacao.
     */
    Pixel operator-( const Pixel& p ) const;

	/** AMK
     * Sobrecarga do operador +.
     * @param p - pixel a ser somado.
     * @return - resultado da operacao.
     */ 
	Pixel operator+( const Pixel& p ) const;


private:
    float _val[3];
};

#endif /* PIXEL_H */

