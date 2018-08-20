//
//  CarnetEntretien.cs
//
//  Author:
//       dimercur <${AuthorEmail}>
//
//  Copyright (c) 2015 dimercur
//
//  This library is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as
//  published by the Free Software Foundation; either version 2.1 of the
//  License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
using System;
using System.Xml;

namespace EntretienAuto
{
	public enum CarnetEntretienStatus
	{
		Success,
		FileNotFound,
		NodeNotFound,
		InvalidDB
	};

	public class CarnetEntretien
	{
		private string carnetEntretienFilename;

		public Vehicule vehicule;
		public Entretien[] entretiens;

		public CarnetEntretien ()
		{
			//doc = new XmlDocument ();
		}

		public CarnetEntretienStatus CreerCarnetVide(string filename)
		{
    		CarnetEntretienStatus status = CarnetEntretienStatus.Success;
			vehicule = new Vehicule ();
			entretiens = new Entretien[1];

			XmlDocument doc = new XmlDocument ();
			XmlDeclaration dec = doc.CreateXmlDeclaration ("1.0", "UTF-8", "");
			doc.AppendChild (dec);

			XmlNode mainNode = doc.CreateNode (XmlNodeType.Document, "EntretienDB", "");
			doc.AppendChild (mainNode);
			doc.Save (filename);

			return status;
		}

		public CarnetEntretienStatus OuvrirCarnet(string filename)
		{
			XmlNodeList VehiculeList;
			XmlNode VehiculeNode;
			XmlNodeList EntretiensList;
			XmlNode EntretiensListNode;
			XmlDocument doc = new XmlDocument ();

			int i = 0;

			/* ouvre le fichier */
			carnetEntretienFilename = filename;

			try	{
				doc.Load (carnetEntretienFilename);
			}
			catch (System.IO.FileNotFoundException e) {
				Console.WriteLine ("Exception while opening file " + carnetEntretienFilename + " : " + e);
				return CarnetEntretienStatus.FileNotFound;
			}
			catch (Exception e) {
				Console.WriteLine ("Exception while opening file " + carnetEntretienFilename + " : " + e);
				return CarnetEntretienStatus.InvalidDB;
			}

			/* Lecture du vehicule */
			try {
				VehiculeNode = doc.SelectSingleNode ("/EntretienDB");
				VehiculeList = VehiculeNode.SelectNodes ("Vehicule");
			}
			catch (Exception e) {
				Console.WriteLine ("Exception while reading node /EntretienDB/Vehicule : " + e);
				return CarnetEntretienStatus.InvalidDB;
			}
				
			vehicule = new Vehicule();

			vehicule.Marque = VehiculeList[0].Attributes.GetNamedItem ("marque").Value;
			vehicule.Modele = VehiculeList[0].Attributes.GetNamedItem ("modele").Value;
			vehicule.Immatriculation = VehiculeList[0].InnerText;

			Console.WriteLine ("Fiche vehicule: " + vehicule.Immatriculation
				+ " [" + vehicule.Marque + ","
				+ vehicule.Modele + "]");
			
			/* Lecture des fiches d'entretien */
			try {
				EntretiensListNode = doc.SelectSingleNode ("/EntretienDB/EntretienList");
				EntretiensList = EntretiensListNode.SelectNodes ("Entretien");
			}
			catch (Exception e) {
				Console.WriteLine ("Exception while reading node /EntretienDB/EntretienList/Entretien : " + e);
				return CarnetEntretienStatus.InvalidDB;
			}

			entretiens = new Entretien[EntretiensList.Count];
			i = 0;
			Console.WriteLine ("Nombre de fiches entretiens: "+ entretiens.Length);

			foreach (XmlNode node in EntretiensList) {
				Entretien Fiche = new Entretien ();

				Fiche.Date = "";
				Fiche.Mecanicien = "";
				Fiche.Titre = "";
				Fiche.Diagnostique = "";
				Fiche.Kilometrage = "";
				Fiche.PiecesDetachees = "";
				Fiche.Prix = "";
				Fiche.Type = "";

				if (node.Attributes.GetNamedItem ("mecanicien")!=null) Fiche.Mecanicien = node.Attributes.GetNamedItem ("mecanicien").Value;
				if (node.Attributes.GetNamedItem ("titre")!=null) Fiche.Titre = node.Attributes.GetNamedItem ("titre").Value;
				if (node.Attributes.GetNamedItem ("diagnostique")!=null) Fiche.Diagnostique = node.Attributes.GetNamedItem ("diagnostique").Value;
				if (node.Attributes.GetNamedItem ("kms")!=null) Fiche.Kilometrage = node.Attributes.GetNamedItem ("kms").Value;
				if (node.Attributes.GetNamedItem ("piecesdetachees")!=null) Fiche.PiecesDetachees = node.Attributes.GetNamedItem ("piecesdetachees").Value;
				if (node.Attributes.GetNamedItem ("prix")!=null) Fiche.Prix = node.Attributes.GetNamedItem ("prix").Value;
				if (node.Attributes.GetNamedItem ("type")!=null) Fiche.Type = node.Attributes.GetNamedItem ("type").Value;
				Fiche.Date = node.InnerText;

				entretiens [i] = Fiche;
				i++;
			}

			for (i = 0; i < entretiens.Length; i++) {
				Console.WriteLine ("Nouvelle fiche: " 
					+ " [" + entretiens[i].Date + ","
					+ entretiens[i].Titre + "]");
			}

			return CarnetEntretienStatus.Success;
		}

		public CarnetEntretienStatus EnregistrerCarnet()
		{
			throw new NotImplementedException ();
		}

		public CarnetEntretienStatus EnregistrerCarnetSous(string filename)
		{
			CarnetEntretienStatus status = CarnetEntretienStatus.Success;

			XmlDocument xmlDoc = new XmlDocument();
			XmlDeclaration dec = xmlDoc.CreateXmlDeclaration ("1.0", "UTF-8", "");
			xmlDoc.AppendChild (dec);

			XmlNode rootNode = xmlDoc.CreateElement("EntretienDB");
			xmlDoc.AppendChild(rootNode);

			XmlNode userNode = xmlDoc.CreateElement("Vehicule");
			XmlAttribute attribute = xmlDoc.CreateAttribute("Marque");
			attribute.Value = vehicule.Marque;
			userNode.Attributes.Append(attribute);
			attribute = xmlDoc.CreateAttribute("Modele");
			attribute.Value = vehicule.Modele;
			userNode.Attributes.Append(attribute);
			userNode.InnerText = vehicule.Immatriculation;
			rootNode.AppendChild(userNode);

			XmlNode EntretienNode = xmlDoc.CreateElement("EntretienList");
			rootNode.AppendChild(EntretienNode);

			foreach (Entretien e in entretiens) {
				XmlNode ficheNode = xmlDoc.CreateElement("Entretien");
				XmlAttribute attr = xmlDoc.CreateAttribute ("Kilometrage");
				attr.Value = e.Kilometrage;
				ficheNode.Attributes.Append (attr);
				attr = xmlDoc.CreateAttribute ("Titre");
				attr.Value = e.Titre;
				ficheNode.Attributes.Append (attr);
				ficheNode.InnerText = e.Date;
				EntretienNode.AppendChild (ficheNode);
			}

			xmlDoc.Save(filename);

			return status;
		}

		public CarnetEntretienStatus  AjouterFiche(Entretien fiche)
		{
			throw new NotImplementedException ();
		}

		public CarnetEntretienStatus SupprimerFiche(int index)
		{
			throw new NotImplementedException ();
		}

		private void TrierCarnet()
		{
			throw new NotImplementedException ();
		}
	}
}

