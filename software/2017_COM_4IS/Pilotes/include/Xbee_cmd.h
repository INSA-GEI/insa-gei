/*!
 * \file
 * \brief 	Bibliotheque de fonctions pour initialiser, envoyer et recevoir de données vers/de un module xbee.
 * \author 	S. DI MERCURIO
 * \date   	septembre 2017
 * \version 1.0
 * \warning Necessite le package \ref xbee_usart
 */

#if !defined (__XBEE_CMD_H__)
#define __XBEE_CMD_H__

/**
 * \page xbee_cmd XBEE_CMD
 * \section XBEE_CMD_intro Présentation
 *
 * Le module xbee_cmd fourni des fonctions pour initialiser, envoyer et recevoir de données vers/de un module xbee.
 *
 * \section XBEE_CMD_dep Dépendances
 * Le module s'appuie sur le pilote \ref xbee_usart
 *
 * \section XBEE_CMD_compinfo Clefs de compilation
 * Les clefs de compilation suivantes peuvent être utilisées:
 *  - __DEBUG__ : Permet d'afficher dans la fenetre "serial windows (debug)" les données envoyées ou recues du module xbee 
 *
 * \section XBEE_CMD_reference Liste des API relatif à ce pilote
 *  - \ref XBEE_CMD_module
 *
 * \section XBEE_CMD_exemples Exemples d'utilisation
 *  Exemple: Initialise lle module xbee, envoit puis recoit des données
 *
 *  \code
 *  #include "Xbee_cmd.h"
 *  
 *  int status;
 *  int address;
 *  char buffer[101];
 *
 *  void main (void)
 *  {
 *  	xbee_init(0x0D, 0xACC0, 0x20);
 *
 *		status=xbee_send_data (0x10, "Hello");
 *	
 *		xbee_set_timeout(60*XBEE_TIMEOUT_1S); // timeout reglé sur 1 minute (60s)
 *		status=xbee_get_data(&address, (char*)buffer);
 *
 *    if (status==XBEE_SUCCESS)
 *		{
 *			// Des données ont été reçues, à traiter
 *		}
 *		else
 *		{
 *			// Soit on n'a pas reçu de données (XBEE_TIMEOUT), soit les données etaient corrompues (XBEE_INVALID_FRAME)
 *		}
 *  }
 *
 *  \endcode
 */

/**
 * @addtogroup XBEE_CMD_module XBEE_CMD
 * @{
 */

	/** @addtogroup XBEE_CMD_constantes Constantes
	 * @{
	 */
	 
/**
 * Liste des codes retour possibles pour les fonctions
 */
enum XBEE_STATUS {
	XBEE_SUCCESS=		  	0, /*!< L'operation s'est déroulée sans probleme */
	XBEE_INVALID_CHANNEL= 	1, /*!< La valeur de canal ne fait pas partie des valeurs autorisées (0x0B - 0x1C) */
	XBEE_INVALID_PANID= 	2, /*!< La valeur de pan id ne fait pas partie des valeurs autorisées (0x0000 - 0xFFFF) */
	XBEE_INVALID_ADDRESS=	3, /*!< La valeur d'adresse source ne fait pas partie des valeurs autorisées (0x0000 - 0xFFFF) */
	XBEE_TIMEOUT_EXPIRED= 	4, /*!< L'operation a pris plus de temps que la durée maximum (timeout) \see XBEE_TIMEOUT \see xbee_set_timeout */
	XBEE_INVALID_FRAME=  	5, /*!< Les données recues sont invalides */
	XBEE_INIT_FAILURE=		6, /*!< L'initalisation du xbee a echouée \see xbee_init */
	XBEE_BUSY=				7, /*!< Le xbee est déjà occupée */
	XBEE_SEND_FAILED=		8  /*!< L'envoi des données a échouée */
};

/**
 * Definitions pour gerer l'attente lors de la reception de données
 * \see xbee_set_timeout
 * \see xbee_get_data
 */
enum XBEE_TIMEOUT {
	XBEE_NO_TIMEOUT = 0,        /*!< Defini une attente bloquante, sans limite de temps */
	XBEE_TIMEOUT_1S = 0x198000  /*!< Defini une attente maximum de 1s */
};

	/**
	 * @}
	 */

    /** @addtogroup XBEE_CMD_fonctions Fonctions
	 * @{
	 */

/**
 * Initialise le module XBEE
 * @param channel Le canal d'emission (0x0B-0x1D).
 * @param pan_id Le pan identifier du reseau (0x0000 - 0xFFFF).
 * @param address L'adresse propre (source) du module (0x0000 - 0xFFFE).
 * @return Resultat de l'init.
 * @see XBEE_STATUS
 */ 
int xbee_init(int channel, int pan_id, int address);

/**
 * Envoi une chaine de caractere a un module xbee identifié apr son adresse 
 * @param addr_destination Adresse du module xbee destinataire.
 * @param str Pointeur sur une chaine de caractere.
 * @return Resultat de l'envoi.
 * @see XBEE_STATUS
 */ 
int xbee_send_data(int addr_destination, char *str);

/**
 * Recupere une chaine de caractere envoyée par un module xbee
 * @param addr_source Adresse du module xbee emmeteur (rensseigné par la fonction).
 * @param str Pointeur sur un buffer préalablement alloué, suffisament grand pour stocker les données recues (max 101 caracteres).
 * @return Resultat de la reception.
 * @see XBEE_STATUS
 * \remark Cette fonction est affectée par la valeur de timeout reglée avec \ref xbee_set_timeout. Dans le cas ou le timeout est reglé
 *         avec XBEE_NO_TIMEOUT, la fonction est bloquante sans limite de temps
 */ 
int xbee_get_data(int *addr_source, char *str);

/**
 * Regle la durée d'attente maximum (timeout) lors de la reception de donnée
 * @param timeout Durée d'attente max.
 * \see XBEE_TIMEOUT
 * \remark Il est possible d'ajouter un coefficient multiplicateur lors du passage de parametre (ex: xbee_set_timeout(3*XBEE_TIMEOUT_1S) pour regler un timeout de 3s
 */ 
void xbee_set_timeout(long long timeout);

/**
 * @}
 */
#endif /* __XBEE_CMD_H__ */
