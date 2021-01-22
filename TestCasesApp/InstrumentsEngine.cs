using FileHelpers;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  /// <summary>
  /// 
  /// </summary>
  public class InstrumentsEngine
  {
    private static readonly Lazy<InstrumentsEngine> lazy = new Lazy<InstrumentsEngine>(() => new InstrumentsEngine());

    public static InstrumentsEngine Instance { get { return lazy.Value; } }

    /// <summary>
    /// 
    /// </summary>
    private InstrumentsEngine()
    {
      engine.ErrorMode = ErrorMode.SaveAndContinue;
      engine.AfterReadRecord += AfterReadRecord;

      engineSync.ErrorMode = ErrorMode.SaveAndContinue;
    }

    /// <summary>
    /// read data
    /// </summary>
    /// <param name="directory">path of instruments file</param>
    public List<Instrument> Execute(string directory)
    {
      string file = $"{directory}\\instruments.dat";
      //--- checks
      if (!File.Exists(file))
        return (null);
      // Read
      var list = new List<Instrument>();
      using (engine.BeginReadFile(file))
      {
        // The engine is IEnumerable
        foreach (Instrument instrument in engine)
        {
          list.Add(instrument);
        }
      }
      //---
      return list;
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="directory"></param>
    /// <param name="instruments"></param>
    public void Update(string directory, List<Instrument> instruments)
    {
      //--- checks
      if(instruments.Count<1||string.IsNullOrEmpty(directory))
        return;
      //---
      string file = $"{directory}\\instruments.dat";

      var copy = new List<Instrument>();
      foreach (var elt in instruments)
      {
        copy.Add((Instrument)elt.Clone());
      }

      foreach (var data in copy)
      {
        data.Symbol = $"{data.Symbol}={data.ShortName}";
      }

      engineSync.WriteFile(file, copy);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="engine"></param>
    /// <param name="e"></param>
    private void AfterReadRecord(EngineBase engine, FileHelpers.Events.AfterReadEventArgs<Instrument> e)
    {
      var split = e.Record.Symbol.Split('=');
      if (split.Length == 2)
      {
        e.Record.Symbol = split[0];
        e.Record.ShortName = split[1];
      }
    }

    private FileHelperAsyncEngine<Instrument> engine = new FileHelperAsyncEngine<Instrument>();
    private FileHelperEngine<Instrument> engineSync = new FileHelperEngine<Instrument>();
  }
}
