/*!
 * \file
 * \brief 	Pilote de liaison serie pour le module \ref fm_cmd.
 * \author 	S. DI MERCURIO
 * \date   	septembre 2017
 * \version 1.0
 */
 
#if !defined (__FM_USART_H__)
#define __FM_USART_H__

/**
 * \page xbee_usart FM_USART
 * \section FM_USART_intro Présentation
 *
 * Le module fm_usart fourni des fonctions pour initialiser l'usart, envoyer et recevoir de données sur l'usart.
 *
 * \section FM_USART_dep Dépendances
 * Pas de dépendance
 *
 * \section FM_USART_compinfo Clefs de compilation
 * Pas de clef de compilation
 *
 * \section FM_USART_reference Liste des API relatif à ce pilote
 *  - \ref FM_USART_module
 */

/**
 * @addtogroup FM_USART_module FM_USART
 * @{
 */

	/** @addtogroup FM_USART_constantes Constantes
	 * @{
	 */

/**
 * Liste des codes retour possibles pour les fonctions
 */
enum FM_USART_STATUS {
	FM_USART_SUCCESS=0, /*!< L'operation s'est déroulée sans probleme */
	FM_USART_BUSY,      /*!< La liaison serie est déjà occupée à transmettre des données */
	FM_USART_IDLE       /*!< La liaison serie est disponible */
};

/** 
 * Definition des differents mode de fonctionnement du module FM
 */
enum FM_MODE {
	FM_OFF=0, 				/*!< Module FM inactif */
	FM_TX,            /*!< Module FM en transmission */
	FM_RX, 				    /*!< Module FM en reception */
	FM_TX_LOOPBACK		/*!< Module FM en loopback (mode test) */
};

	/**
	 * @}
	 */

    /** @addtogroup FM_USART_fonctions Fonctions
	 * @{
	 */

/**
 * Initialise la liaison serie
 * @param baudrate Vitesse de transmission (baudrate). Classiquement, le module FM fonctionne à 9600 bauds.
 * @param priority Priorité de l'interruption USART. Valeur entre 0 et 15, plus la valeur est petite, plus l'interruption est prioritaire.
 */ 
void fm_init_usart(int baudrate, char priority);

/**
 * Enregistre une fonction callback qui sera appelée lors de la reception d'un caractere
 * @param func_ptr Pointeur sur une fonction dont le prototype est void <func>(char data).
 */ 
void fm_set_receive_callback(void (*func_ptr) (char data));

/**
 * Enregistre une fonction callback qui sera appelée lorsque la transmission sera completement terminée
 * @param func_ptr Pointeur sur une fonction dont le prototype est void <func>(void).
 */ 
void fm_set_transmit_empty_callback(void (*func_ptr) (void));

/**
* Change le mode de fonctionnement du module FM: Emetteur, recepteur, inactif 
 * @param mode Mode fonctionnement. \see FM_MODE
 */ 
void fm_set_mode(int mode);
	
/**
 * Envoi un buffer de caractere sur la liaison serie
 * @param buffer Pointeur sur un buffer de caractere
 * @param length Longueur des données à envoyer
 * @return Resultat de l'envoi.
 * @see FM_USART_STATUS
 */ 
int fm_send_data_usart(char *buffer, int length);

/**
 * Verifie si l'usart est libre (IDLE)
 * @return Etat de la liaison serie.
 * @see FM_USART_STATUS
 * @see FM_USART_BUSY
 * @see FM_USART_IDLE
 */ 
int fm_is_busy_usart(void);

/**
 * Retourne l'état de la porteuse
 * @return Etat de la porteuse: 1 = porteuse detectée, 0 = porteuse absente.
 */ 
int fm_get_carrier_low_level(void);

/**
 * @}
 */
 
#endif /* __FM_USART_H__ */
