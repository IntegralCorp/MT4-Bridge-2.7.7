using FileHelpers;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  public class PriceConvertor: ConverterBase
  {
    /// <summary>
    /// current settings
    /// </summary>
    private static readonly NumberFormatInfo m_Provider = new NumberFormatInfo { NumberDecimalSeparator = "." };

    public override object StringToField(string from)
    {
      from = from.Replace("price", "").Trim();
      return Convert.ToDouble(Double.Parse(from,m_Provider));
    }

    public override string FieldToString(object fieldValue)
    {
      return ((double)fieldValue).ToString("#.###000",m_Provider).Insert(0,"price ");
    }
  }
}
