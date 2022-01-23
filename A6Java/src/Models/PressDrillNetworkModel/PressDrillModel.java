package Models.PressDrillNetworkModel;

import Models.AtomicModel;

public abstract class PressDrillModel extends AtomicModel<Integer, Integer> {

  public PressDrillModel(String n, int t) {
    super(n,t);
  }

  //functions
  @Override
  public Integer lambda() {
    return 1;
  }
  
  @Override
  public void deltaInt() {
    p--;
    s = this.processingTime;
  }

  @Override
  public void deltaCon(Integer q) {
    p += q - 1;
    s = this.processingTime;
  }

  @Override
  public void deltaExt(Integer q, double e) {
    if (p > 0) {
      p += q;
      s -= e;
    } else {
      p += q;
      s = this.processingTime;
    }
  }

  // use this to create new events. You will add this value to your next event's output time
  @Override
  public double timeAdvance() {
    if (p > 0)
      return s;
    return Double.MAX_VALUE;
  }
}
