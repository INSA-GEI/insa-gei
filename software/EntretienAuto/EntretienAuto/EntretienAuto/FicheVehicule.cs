using System;
using EntretienAuto;


public partial class FicheVehicule : Gtk.Dialog
{
	public int status=-1;

	public FicheVehicule ()
	{
		this.Build ();
	}

	public Vehicule Lire()
	{
		Vehicule info = new Vehicule ();

		info.Marque = comboboxentryMarque.ActiveText;
		info.Modele = comboboxentryModele.ActiveText;
		info.Immatriculation = entryImmatriculation.Text;
		 
		return info;
	}
}


