using FileHelpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  [DelimitedRecord("\t")]
  public class CFixMessage
  {
    [FieldOptional]
    [FieldNullValue(typeof(string), "")]
    public string Time { get; set; }

    [FieldOptional]
    [FieldNullValue(typeof(string), "")]
    public string Type { get; set; }

    [FieldOptional]
    [FieldNullValue(typeof(string), "")]
    public string Dummy { get; set; }

    [FieldOptional]
    [FieldNullValue(typeof(string), "")]
    public string Context { get; set; }

    [FieldOptional]
    [FieldNullValue(typeof(string), "")]
    public string Message { get; set; }

    [FieldOptional]
    [FieldNullValue(typeof(string), "")]
    public string InOut { get; set; }
  }
}
