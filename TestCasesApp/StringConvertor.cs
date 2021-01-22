using FileHelpers;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  public class StringConvertor : ConverterBase
  {
    public override object StringToField(string from)
    {
      from = from.Replace("FXI Order ID", "").Replace("FXI Execution ID", "").Trim();
      return from;
    }

    public override string FieldToString(object fieldValue)
    {
      return ((string)fieldValue).Insert(0, "FXI Order ID ");
    }
  }
}
