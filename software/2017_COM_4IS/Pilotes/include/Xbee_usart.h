/*!
 * \file
 * \brief 	Pilotes de liaison serie pour le paquetage \ref xbee_cmd
 * \author 	S. DI MERCURIO
 * \date   	Septembre 2017
 * \version 1.0
 * \remark  Ne fonctionne qu'avec les cartes reseau de capteur: numero de peripherique USART figé (USART2)
 */
 
#if !defined (__XBEE_USART_H__)
#define __XBEE_USART_H__

/**
 * \page xbee_usart_ XBEE_USART
 * \section XBEE_USART_intro Présentation
 *
 * Le module xbee_usart fourni des fonctions pour initialiser l'usart, envoyer et recevoir de données sur l'usart.
 *
 * \section XBEE_USART_dep Dépendances
 * Pas de dépendance
 *
 * \section XBEE_USART_compinfo Clefs de compilation
 * Pas de clef de compilation
 *
 * \section XBEE_USART_reference Liste des API relatif à ce pilote
 *  - \ref XBEE_USART_module
 */

/**
 * @addtogroup XBEE_USART_module XBEE_USART
 * @{
 */

	/** @addtogroup XBEE_USART_constantes Constantes
	 * @{
	 */

/**
 * Liste des codes retour possibles pour les fonctions
 */
enum XBEE_USART_STATUS {
	XBEE_USART_SUCCESS=0, /*!< L'operation s'est déroulée sans probleme */
	XBEE_USART_BUSY,      /*!< La liaison serie est déjà occupée à transmettre des données */
	XBEE_USART_IDLE       /*!< La liaison serie est disponible */
};

	/**
	 * @}
	 */

    /** @addtogroup XBEE_USART_fonctions Fonctions
	 * @{
	 */

/**
 * Initialise la liaison serie
 * @param baudrate Vitesse de transmission (baudrate). Classiquement, le xbee fonctionne à 9600 bauds ou 115200 bauds.
 * @param priority Priorité de l'interruption USART. Valeur entre 0 et 15, plus la valeur est petite, plus l'interruption est prioritaire.
 */ 
void xbee_init_usart(int baudrate, char priority);

/**
 * Enregistre une fonction callback lors de la reception d'un caractere
 * @param func_ptr Pointeur sur une fonction dont le prototype est void <func>(char data).
 */ 
void xbee_set_callback(void (*func_ptr) (char data));

/**
 * Envoi un buffer de caractere sur la liaison serie
 * @param buffer Pointeur sur un buffer de caractere
 * @param length Longueur des données à envoyer
 * @return Resultat de l'envoi.
 * @see XBEE_USART_STATUS
 */ 
int xbee_send_data_usart(char *buffer, int length);

/**
 * Verifie si l'usart est libre (IDLE)
 * @return Etat de la liaison serie.
 * @see XBEE_USART_STATUS
 * @see XBEE_USART_BUSY
 * @see XBEE_USART_IDLE
 */ 
int xbee_is_busy_usart(void);

/**
 * @}
 */
#endif /* __XBEE_USART_H__ */

