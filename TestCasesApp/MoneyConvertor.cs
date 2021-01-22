using FileHelpers;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  public class MoneyConvertor : ConverterBase
  {
    /// <summary>
    /// current settings
    /// </summary>
    private static readonly NumberFormatInfo m_Provider = new NumberFormatInfo { NumberDecimalSeparator = "." };

    public override object StringToField(string from)
    {
      from = from.Replace("k", "000").Replace("K", "000").Replace("m", "000000").Replace("M", "000000").Trim();
      return Convert.ToDouble(Double.Parse(from, m_Provider));
    }

    public override string FieldToString(object fieldValue)
    {
      string formatted;
      double num = (double)fieldValue;
      if (num >= 1000000)
      {
        formatted = ((double)num / 1000000.0).ToString("N0") + "M";
      }
      else if (num >= 1000)
      {
        formatted = ((double)num / 1000.0).ToString("N0") + "k";
      }
      else
        formatted = num.ToString("N0");
      //---
      return formatted;
    }
  }
}
