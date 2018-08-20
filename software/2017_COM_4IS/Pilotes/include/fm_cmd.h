/*!
 * \file
 * \brief 	Bibliotheque de fonctions pour initialiser, envoyer et recevoir de données vers/de un module FM (RT606).
 * \author 	S. DI MERCURIO
 * \date   	septembre 2017
 * \version 1.0
 * \warning Necessite le package \ref fm_usart
 */
 
#if !defined (__FM_CMD_H__)
#define __FM_CMD_H__


/**
 * \page fm_cmd FM_CMD
 * \section FM_CMD_intro Présentation
 *
 * Le module fm_cmd fourni des fonctions pour initialiser, envoyer et recevoir de données vers/de un module fm.
 *
 * \section FM_CMD_dep Dépendances
 * Le module s'appuie sur le pilote \ref fm_usart
 *
 * \section FM_CMD_compinfo Clefs de compilation
 * Les clefs de compilation suivantes peuvent être utilisées:
 *  - __DEBUG__ : Permet d'afficher dans la fenetre "serial windows (debug)" les données envoyées ou recues du module fme 
 *
 * \section FM_CMD_reference Liste des API relatif à ce pilote
 *  - \ref FM_CMD_module
 *
 * \section FM_CMD_exemples Exemples d'utilisation
 *  Exemple: Initialise le module fm, envoit puis recoit des données
 *
 *  \code
 *  #include "fm_cmd.h"
 *  
 *  int status;
 *  char buffer[101];
 *
 *  void main (void)
 *  {
 *  	fm_init();
 *
 *		status=fm_send_data ("Hello");
 *	
 *		fm_set_timeout(60*XBEE_TIMEOUT_1S); // timeout reglé sur 1 minute (60s)
 *		status=fm_get_data((char*)buffer);
 *
 *    if ((status == FM_SUCCESS) && (fm_get_carrier()==FM_CARRIER_DETECTED))
 *    {
 *			// Les données reçues dans buffer sont valides (a priori, la porteuse n'étant pas super fiable)
 *    }
 *		else
 *		{
 *  		// Soit on n'a pas reçu de donnée (FM_TIMEOUT) soit la porteuse n'est pas presente et les données invalides
 *		}
 *  }
 *
 *  \endcode
 */

/**
 * @addtogroup FM_CMD_module FM_CMD
 * @{
 */

	/** @addtogroup FM_CMD_constantes Constantes
	 * @{
	 */
	 
/**
 * Liste des codes retour possibles pour les fonctions
 */
enum FM_STATUS {
	FM_SUCCESS=		  			0, /*!< L'operation s'est déroulée sans probleme */
	FM_TIMEOUT_EXPIRED= 	1, /*!< L'operation a pris plus de temps que la durée maximum (timeout) \see XBEE_TIMEOUT \see fm_set_timeout */
	FM_INVALID_BUFFER=    2, /*!< Le buffer passé en parametre est invalide (ptr =0) */
	FM_BUSY=							3  /*!< Le module fm est déjà occupée */
};

/**
 * Definitions pour gerer l'attente lors de la reception de données
 * \see fm_set_timeout
 * \see fm_get_data
 */
enum FM_TIMEOUT {
	FM_NO_TIMEOUT = 0,        /*!< Defini une attente bloquante, sans limite de temps */
	FM_TIMEOUT_1S = 0x198000  /*!< Defini une attente maximum de 1s */
};

/**
 * Definitions pour l'etat de la porteuse (carrier)
 * \see fm_get_carrier
 */
enum FM_CARRIER {
	FM_CARRIER_DETECTED = 0,	/*!< Une porteuse a été detectée */
	FM_NO_CARRIER  						/*!< Aucune porteuse detectée */
};
	/**
	 * @}
	 */

    /** @addtogroup FM_CMD_fonctions Fonctions
	 * @{
	 */

/**
 * Initialise le module FM
 * @return Resultat de l'initialisation.
 * @see FM_STATUS
 */ 
int fm_init(void);

/**
 * Envoi une chaine de caractere via le module FM
 * @param str Pointeur sur une chaine de caractere.
 * @return Resultat de l'envoi.
 * @see FM_STATUS
 */ 
int fm_send_data(char *str);

/**
 * Recupere une chaine de caractere recu par le module FM
 * @param str Pointeur sur un buffer préalablement alloué, suffisament grand pour stocker les données recues (max 101 caracteres).
 * @return Resultat de la reception.
 * @see FM_STATUS
 * \remark Cette fonction est affectée par la valeur de timeout reglée avec \ref fm_set_timeout. Dans le cas ou le timeout est reglé
 *         avec FM_NO_TIMEOUT, la fonction est bloquante sans limite de temps
 */ 
int fm_get_data(char *str);

/**
 * Regle la durée d'attente maximum (timeout) lors de la reception de donnée
 * @param timeout Durée d'attente max.
 * \see FM_TIMEOUT
 * \remark Il est possible d'ajouter un coefficient multiplicateur lors du passage de parametre (ex: xbee_set_timeout(3*FM_TIMEOUT_1S) pour regler un timeout de 3s
 */ 
void fm_set_timeout(long long timeout);

/**
 * Verifie l'état de la porteuse
 * @return Etat de la porteuse
 * \see FM_CARRIER
 * \remark L'etat de la porteuse n'est pas quelque chose de très fiable. Perferez une detecttion de donnée valide par pattern
 */ 
int fm_get_carrier(void);

/**
 * @}
 */
 
#endif /* __FM_CMD_H__ */
