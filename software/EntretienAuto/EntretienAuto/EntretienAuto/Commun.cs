using System;

namespace EntretienAuto
{
	public class Vehicule
	{
		public string Marque { get; set; }
		public string Modele { get; set; }
		public string Immatriculation { get; set; }
		public string DataAchat { get; set; }
		public string DateMiseEnCirculation { get; set; }
		public string DateVente { get; set; }
		public string NumeroChassis { get; set; }
		public string NumeroMines { get; set; }
		public string Couleur { get; set; }
		public string Energie { get; set; }
		public string TypeMoteur { get; set; }
		public string Serie { get; set; }
		public string PuissanceFiscale { get; set; }
		public string PuissanceKW { get; set; }
		public string PuissanceCV { get; set; }
		public string NumerosDivers { get; set; }
	}

	public class Entretien
	{
		public string Date { get; set; }
		public string Mecanicien { get; set; }
		public string Titre { get; set; }
		public string Diagnostique { get; set; }
		public string Prix { get; set; }
		public string PiecesDetachees { get; set; }
		public string Kilometrage { get; set; }
		public string Type { get; set; }
	}

}

