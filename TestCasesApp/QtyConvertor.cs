using FileHelpers;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  public class QtyConvertor : ConverterBase
  {
    /// <summary>
    /// current settings
    /// </summary>
    private static readonly NumberFormatInfo m_Provider = new NumberFormatInfo { NumberDecimalSeparator = "." };

    public override object StringToField(string from)
    {
      from = from.Replace("CumQty", "").Trim();
      return Convert.ToDouble(Double.Parse(from,m_Provider));
    }

    public override string FieldToString(object fieldValue)
    {
      return ((double)fieldValue).ToString("#.##", m_Provider).Insert(0, "CumQty ");
    }
  }
}
