using System;

using sharpPDF.Exceptions;
using sharpPDF.Enumerators;

namespace sharpPDF
{
	/// <summary>
	/// Class that represents the style of a table's row
	/// </summary>
	public class pdfTableRowStyle
	{
		
		private predefinedFont _fontType;
		private int _fontSize;
		private pdfColor _fontColor;
		private pdfColor _bgColor;

		/// <summary>
		/// Type of the Font
		/// </summary>
		public predefinedFont fontType
		{
			get
			{
				return _fontType;
			}			
		}

		/// <summary>
		/// Size of the Font
		/// </summary>
		public int fontSize
		{
			get
			{
				return _fontSize;
			}
		}

		/// <summary>
		/// Color of the Font
		/// </summary>
		public predefinedColor fontColor
		{
			get
			{
				return _fontColor.baseColor;
			}
		}

		/// <summary>
		/// Color of the BackGround
		/// </summary>
		public predefinedColor bgColor
		{
			get
			{
				return _bgColor.baseColor;
			}
		}

		/// <summary>
		/// Class's constructor
		/// </summary>
		public pdfTableRowStyle(predefinedFont fontType, int fontSize, predefinedColor fontColor, predefinedColor bgColor)
		{		
			_fontType = fontType;
			_fontSize = fontSize;
			_fontColor = new pdfColor(fontColor);
			_bgColor = new pdfColor(bgColor);
		}
	}
}
