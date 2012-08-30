void vector_add_cpu (const float* src_a,
               const float* src_b,
               float*  res,
               const int num)
{
   for (int i = 0; i < num; i++)
      res[i] = src_a[i] + src_b[i];
}
