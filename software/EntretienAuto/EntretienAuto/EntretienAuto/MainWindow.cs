﻿//
//  MainWindow.cs
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
using Gtk;
using System.Drawing.Printing;
using System.Drawing;
using System.Xml;
using sharpPDF;
using EntretienAuto;

public partial class MainWindow: Gtk.Window
{
	CarnetEntretien database;
	const string databaseName = "Entretien.xml";

	public MainWindow () : base (Gtk.WindowType.Toplevel)
	{
		CarnetEntretienStatus status;

		Build ();

		database = new CarnetEntretien ();
		status = database.OuvrirCarnet (databaseName);
		if (status != CarnetEntretienStatus.Success) {
			MessageDialog msg = new MessageDialog (this, DialogFlags.Modal,
				                    MessageType.Error, ButtonsType.Cancel, "Fichier " + databaseName + " introuvable ou invalide.\nImpossible de continuer.");
			msg.Title = "Erreur fatale";
			msg.Run();       
			msg.Destroy(); 
		}

		FillEntretienTreeView (database);
	}

	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}

	private void FillEntretienTreeView(CarnetEntretien db)
	{
		TreeViewColumn DateColumn = new TreeViewColumn ();
		DateColumn.Title = "Date";

		TreeViewColumn KilometrageColumn = new TreeViewColumn ();
		KilometrageColumn.Title = "Kilometrage";

		TreeViewColumn TitreColumn = new TreeViewColumn ();
		TitreColumn.Title = "Titre";
		TitreColumn.Resizable = true;

		TreeViewColumn ResponsableColumn = new TreeViewColumn ();
		ResponsableColumn.Title = "Responsable";

		TreeViewColumn DiagColumn = new TreeViewColumn ();
		DiagColumn.Title = "Diagnostique";
		DiagColumn.Resizable = true;

		TreeViewColumn PiecesColumn = new TreeViewColumn ();
		PiecesColumn.Title = "Pieces détachées";
		PiecesColumn.Resizable = true;

		TreeViewColumn CoutColumn = new TreeViewColumn ();
		CoutColumn.Title = "Coût";

		EntretienTreeview.AppendColumn (DateColumn);
		EntretienTreeview.AppendColumn (KilometrageColumn);
		EntretienTreeview.AppendColumn (TitreColumn);
		EntretienTreeview.AppendColumn (ResponsableColumn);
		EntretienTreeview.AppendColumn (DiagColumn);
		EntretienTreeview.AppendColumn (PiecesColumn);
		EntretienTreeview.AppendColumn (CoutColumn);

		ListStore EntretienListStore = new ListStore (typeof(string), typeof(string),
			                               typeof(string), typeof(string),
										   typeof(string), typeof(string),
										   typeof(string));

		EntretienTreeview.Model = EntretienListStore;

		CellRendererText DateCell = new CellRendererText ();
		DateColumn.PackStart (DateCell, true);
		CellRendererText KilometrageCell = new CellRendererText ();
		KilometrageColumn.PackStart (KilometrageCell, true);
		CellRendererText TitreCell = new CellRendererText ();
		TitreColumn.PackStart (TitreCell, true);
		CellRendererText ResponsableCell = new CellRendererText ();
		ResponsableColumn.PackStart (ResponsableCell, true);
		CellRendererText DiagCell = new CellRendererText ();
		DiagColumn.PackStart (DiagCell, true);
		CellRendererText PiecesCell = new CellRendererText ();
		PiecesColumn.PackStart (PiecesCell, true);
		CellRendererText CoutCell = new CellRendererText ();
		CoutColumn.PackStart (CoutCell, true);

		DateColumn.AddAttribute (DateCell, "text", 0); 
		KilometrageColumn.AddAttribute (KilometrageCell, "text", 1);
		TitreColumn.AddAttribute (TitreCell, "text", 2);
		ResponsableColumn.AddAttribute (ResponsableCell, "text", 3);
		DiagColumn.AddAttribute (DiagCell, "text", 4);
		PiecesColumn.AddAttribute (PiecesCell, "text", 5);
		CoutColumn.AddAttribute (CoutCell, "text", 6);

		foreach (CarnetEntretien.Entretien e in db.entretienArray) {
			EntretienListStore.AppendValues (e.Date.ToString (), e.Kilometrage.ToString(),e.Titre.ToString(),
				e.Mecanicien.ToString(), e.Diagnostique.ToString(), e.PiecesDetachees.ToString(), e.Prix.ToString());
		}
	}

	protected void OnVoitureActionActivated (object sender, EventArgs e)
	{
		throw new NotImplementedException ();
	}
}
