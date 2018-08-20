using System;
using System.Text;

using sharpPDF.Exceptions;
using sharpPDF.Enumerators;

namespace sharpPDF
{
	/// <summary>
	/// A Class that implements a PDF Outlines.
	/// </summary>
	internal class pdfOutlines : IWritable
	{
		
		private int _objectIDOutlines;

		/// <summary>
		/// Outlines's ID
		/// </summary>
		public int objectIDOutlines
		{
			get
			{
				return _objectIDOutlines;
			}

			set
			{
				_objectIDOutlines = value;
			}
		}

		/// <summary>
		/// Class's constructor
		/// </summary>
		public pdfOutlines()
		{
			
		}

		/// <summary>
		/// Method that returns the PDF codes to write the Outlines in the document
		/// </summary>
		/// <returns>String that contains PDF codes</returns>
		public string getText() 
		{
			StringBuilder strOutlines = new StringBuilder();	
			strOutlines.Append(_objectIDOutlines.ToString() + " 0 obj" + Convert.ToChar(13) + Convert.ToChar(10));
            strOutlines.Append("<<" + Convert.ToChar(13) + Convert.ToChar(10));
            strOutlines.Append("/Type /Outlines" + Convert.ToChar(13) + Convert.ToChar(10));
            strOutlines.Append("/Count 0" + Convert.ToChar(13) + Convert.ToChar(10));
            strOutlines.Append(">>" + Convert.ToChar(13) + Convert.ToChar(10));
            strOutlines.Append("endobj" + Convert.ToChar(13) + Convert.ToChar(10));
			return strOutlines.ToString();
		}
	}
}
